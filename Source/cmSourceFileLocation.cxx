/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2000-2009 Kitware, Inc., Insight Software Consortium

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#include "cmSourceFileLocation.h"

#include "cmMakefile.h"
#include "cmLocalGenerator.h"
#include "cmGlobalGenerator.h"
#include "cmSystemTools.h"

#include "assert.h"

//----------------------------------------------------------------------------
cmSourceFileLocation::cmSourceFileLocation()
  : Makefile(0), AmbiguousDirectory(true), AmbiguousExtension(true)
{

}

//----------------------------------------------------------------------------
cmSourceFileLocation::cmSourceFileLocation(const cmSourceFileLocation& loc)
  : Makefile(loc.Makefile)
{
  this->AmbiguousDirectory = loc.AmbiguousDirectory;
  this->AmbiguousExtension = loc.AmbiguousExtension;
  this->Directory = loc.Directory;
  this->Name = loc.Name;
}

//----------------------------------------------------------------------------
cmSourceFileLocation&
cmSourceFileLocation::operator=(const cmSourceFileLocation& loc)
{
  if(this == &loc)
    {
    return *this;
    }
  this->Makefile = loc.Makefile;
  this->AmbiguousDirectory = loc.AmbiguousDirectory;
  this->AmbiguousExtension = loc.AmbiguousExtension;
  this->Directory = loc.Directory;
  this->Name = loc.Name;
  this->UpdateExtension(this->Name);
  return *this;
}

//----------------------------------------------------------------------------
cmSourceFileLocation
::cmSourceFileLocation(cmMakefile const* mf, const std::string& name)
  : Makefile(mf)
{
  this->AmbiguousDirectory = !cmSystemTools::FileIsFullPath(name);
  this->AmbiguousExtension = true;
  this->Directory = cmSystemTools::GetFilenamePath(name);
  if (cmSystemTools::FileIsFullPath(this->Directory))
    {
    this->Directory
                  = cmSystemTools::CollapseFullPath(this->Directory);
    }
  this->Name = cmSystemTools::GetFilenameName(name);
  this->UpdateExtension(name);
  return;

  cmake::MessageType messageType = cmake::FATAL_ERROR;
  cmPolicies::PolicyStatus status = mf->GetPolicyStatus(cmPolicies::CMP0054);
  switch(status)
    {
    case cmPolicies::OLD:
      break;
    case cmPolicies::WARN:
      messageType = cmake::AUTHOR_WARNING;
    case cmPolicies::REQUIRED_IF_USED:
    case cmPolicies::REQUIRED_ALWAYS:
    case cmPolicies::NEW:
      if (this->AmbiguousExtension && this->Name != name)
        {
        std::string e = mf->GetPolicies()->
            GetPolicyWarning(cmPolicies::CMP0054);
        e += "\n given path  : " + name;
        e += "\n derived path: " + this->Name;
        this->Makefile->IssueMessage(messageType, e + "\n");
        }
      break;
    }
  if (status != cmPolicies::OLD &&
      status != cmPolicies::WARN)
    {
    this->AmbiguousExtension = true;
    }
}

//----------------------------------------------------------------------------
void cmSourceFileLocation::Update(cmSourceFileLocation const& loc)
{
  if(this->AmbiguousDirectory && !loc.AmbiguousDirectory)
    {
    this->Directory = loc.Directory;
    this->AmbiguousDirectory = false;
    }
  if(this->AmbiguousExtension && !loc.AmbiguousExtension)
    {
    this->Name = loc.Name;
    this->AmbiguousExtension = false;
    }
}

//----------------------------------------------------------------------------
void cmSourceFileLocation::DirectoryUseSource()
{
  assert(this->Makefile);
  if(this->AmbiguousDirectory)
    {
    this->Directory =
      cmSystemTools::CollapseFullPath(
        this->Directory, this->Makefile->GetCurrentDirectory());
    this->AmbiguousDirectory = false;
    }
}

//----------------------------------------------------------------------------
void cmSourceFileLocation::DirectoryUseBinary()
{
  assert(this->Makefile);
  if(this->AmbiguousDirectory)
    {
    this->Directory =
      cmSystemTools::CollapseFullPath(
        this->Directory, this->Makefile->GetCurrentOutputDirectory());
    this->AmbiguousDirectory = false;
    }
}

//----------------------------------------------------------------------------
void cmSourceFileLocation::UpdateExtension(const std::string& name)
{
  assert(this->Makefile);
  // Check the extension.
  std::string ext = cmSystemTools::GetFilenameLastExtension(name);
  if(!ext.empty()) { ext = ext.substr(1); }

  // The global generator checks extensions of enabled languages.
  cmGlobalGenerator* gg =
    this->Makefile->GetLocalGenerator()->GetGlobalGenerator();
  cmMakefile const* mf = this->Makefile;
  const std::vector<std::string>& srcExts = mf->GetSourceExtensions();
  const std::vector<std::string>& hdrExts = mf->GetHeaderExtensions();
  if(!gg->GetLanguageFromExtension(ext.c_str()).empty() ||
     std::find(srcExts.begin(), srcExts.end(), ext) != srcExts.end() ||
     std::find(hdrExts.begin(), hdrExts.end(), ext) != hdrExts.end())
    {
    // This is a known extension.  Use the given filename with extension.
    this->Name = cmSystemTools::GetFilenameName(name);
    this->AmbiguousExtension = false;
    }
  else
    {
    // This is not a known extension.  See if the file exists on disk as
    // named.
    std::string tryPath;
    if(this->AmbiguousDirectory)
      {
      // Check the source tree only because a file in the build tree should
      // be specified by full path at least once.  We do not want this
      // detection to depend on whether the project has already been built.
      tryPath = this->Makefile->GetCurrentDirectory();
      tryPath += "/";
      }
    if(!this->Directory.empty())
      {
      tryPath += this->Directory;
      tryPath += "/";
      }
    tryPath += this->Name;
    if(cmSystemTools::FileExists(tryPath, true))
      {
      // We found a source file named by the user on disk.  Trust it's
      // extension.
      this->Name = cmSystemTools::GetFilenameName(name);
      this->AmbiguousExtension = false;

      // If the directory was ambiguous, it isn't anymore.
      if(this->AmbiguousDirectory)
        {
        this->DirectoryUseSource();
        }
      }
    }
}

//----------------------------------------------------------------------------
bool
cmSourceFileLocation
::MatchesAmbiguousExtension(cmSourceFileLocation const& loc) const
{
  assert(this->Makefile);
  // This location's extension is not ambiguous but loc's extension
  // is.  See if the names match as-is.
  if(this->Name == loc.Name)
    {
    return true;
    }

  // Check if loc's name could possibly be extended to our name by
  // adding an extension.
  if(!(this->Name.size() > loc.Name.size() &&
       this->Name[loc.Name.size()] == '.' &&
       cmHasLiteralPrefixImpl(this->Name.c_str(),
                              loc.Name.c_str(), loc.Name.size())))
    {
    return false;
    }

  // Only a fixed set of extensions will be tried to match a file on
  // disk.  One of these must match if loc refers to this source file.
  std::string const& ext = this->Name.substr(loc.Name.size()+1);
  cmMakefile const* mf = this->Makefile;
  const std::vector<std::string>& srcExts = mf->GetSourceExtensions();
  if(std::find(srcExts.begin(), srcExts.end(), ext) != srcExts.end())
    {
    return true;
    }
  const std::vector<std::string>& hdrExts = mf->GetHeaderExtensions();
  if(std::find(hdrExts.begin(), hdrExts.end(), ext) != hdrExts.end())
    {
    return true;
    }
  return false;
}

//----------------------------------------------------------------------------
bool cmSourceFileLocation::Matches(cmSourceFileLocation const& loc)
{
  assert(this->Makefile);
  if(this->AmbiguousExtension == loc.AmbiguousExtension)
    {
    // Both extensions are similarly ambiguous.  Since only the old fixed set
    // of extensions will be tried, the names must match at this point to be
    // the same file.
    if(this->Name.size() != loc.Name.size() || this->Name != loc.Name)
      {
      return false;
      }
    }
  else
    {
    const cmSourceFileLocation* loc1;
    const cmSourceFileLocation* loc2;
    if(this->AmbiguousExtension)
      {
      // Only "this" extension is ambiguous.
      loc1 = &loc;
      loc2 = this;
      }
    else
      {
      // Only "loc" extension is ambiguous.
      loc1 = this;
      loc2 = &loc;
      }
    if(!loc1->MatchesAmbiguousExtension(*loc2))
      {
      return false;
      }
    }

  if(!this->AmbiguousDirectory && !loc.AmbiguousDirectory)
    {
    // Both sides have absolute directories.
    if(this->Directory != loc.Directory)
      {
      return false;
      }
    }
  else if(this->AmbiguousDirectory && loc.AmbiguousDirectory)
    {
    if (this->Makefile == loc.Makefile)
      {
      // Both sides have directories relative to the same location.
      if(this->Directory != loc.Directory)
        {
        return false;
        }
      }
    else
      {
      // Each side has a directory relative to a different location.
      // This can occur when referencing a source file from a different
      // directory.  This is not yet allowed.
      this->Makefile->IssueMessage(
        cmake::INTERNAL_ERROR,
        "Matches error: Each side has a directory relative to a different "
        "location. This can occur when referencing a source file from a "
        "different directory.  This is not yet allowed."
        );
      return false;
      }
    }
  else if(this->AmbiguousDirectory)
    {
    // Compare possible directory combinations.
    std::string const& srcDir =
      cmSystemTools::CollapseFullPath(
        this->Directory, this->Makefile->GetCurrentDirectory());
    std::string const& binDir =
      cmSystemTools::CollapseFullPath(
        this->Directory, this->Makefile->GetCurrentOutputDirectory());
    if(srcDir != loc.Directory &&
       binDir != loc.Directory)
      {
      return false;
      }
    }
  else if(loc.AmbiguousDirectory)
    {
    // Compare possible directory combinations.
    std::string const& srcDir =
      cmSystemTools::CollapseFullPath(
        loc.Directory, loc.Makefile->GetCurrentDirectory());
    std::string const& binDir =
      cmSystemTools::CollapseFullPath(
        loc.Directory, loc.Makefile->GetCurrentOutputDirectory());
    if(srcDir != this->Directory &&
       binDir != this->Directory)
      {
      return false;
      }
    }

  // File locations match.
  this->Update(loc);
  return true;
}
