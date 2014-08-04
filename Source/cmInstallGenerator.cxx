/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2000-2009 Kitware, Inc., Insight Software Consortium

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#include "cmInstallGenerator.h"

#include "cmMakefile.h"
#include "cmSystemTools.h"

//----------------------------------------------------------------------------
cmInstallGenerator
::cmInstallGenerator(const std::string& destination,
                     std::vector<std::string> const& configurations,
                     const std::string& component,
                     MessageLevel message):
  cmScriptGenerator("CMAKE_INSTALL_CONFIG_NAME", configurations),
  Destination(destination),
  Component(component),
  Message(message)
{
}

//----------------------------------------------------------------------------
cmInstallGenerator
::~cmInstallGenerator()
{
}

//----------------------------------------------------------------------------
void cmInstallGenerator
::AddInstallRule(
                 std::ostream& os,
                 cmInstallType type,
                 std::vector<std::string> const& files,
                 bool optional /* = false */,
                 const std::string& permissions_file /* = "" */,
                 const std::string& permissions_dir /* = "" */,
                 const std::string& rename /* = "" */,
                 const std::string& literal_args /* = "" */,
                 Indent const& indent
                 )
{
  // Use the FILE command to install the file.
  std::string stype;
  switch(type)
    {
    case cmInstallType_DIRECTORY:      stype = "DIRECTORY"; break;
    case cmInstallType_PROGRAMS:       stype = "PROGRAM"; break;
    case cmInstallType_EXECUTABLE:     stype = "EXECUTABLE"; break;
    case cmInstallType_STATIC_LIBRARY: stype = "STATIC_LIBRARY"; break;
    case cmInstallType_SHARED_LIBRARY: stype = "SHARED_LIBRARY"; break;
    case cmInstallType_MODULE_LIBRARY: stype = "MODULE"; break;
    case cmInstallType_FILES:          stype = "FILE"; break;
    }
  os << indent;
  std::string dest = this->GetInstallDestination();
  if (cmSystemTools::FileIsFullPath(dest))
     {
     os << "list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES\n";
     os << indent << " \"";
     for(std::vector<std::string>::const_iterator fi = files.begin();
               fi != files.end(); ++fi)
             {
               if (fi!=files.begin())
                 {
                 os << ";";
                 }
               os << dest << "/";
               if (!rename.empty())
                 {
                 os << rename;
                 }
               else
                 {
                 os << cmSystemTools::GetFilenameName(*fi);
                 }
             }
     os << "\")\n";
     os << indent << "if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)\n";
     os << indent << indent << "message(WARNING \"ABSOLUTE path INSTALL "
        << "DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}\")\n";
     os << indent << "endif()\n";

     os << indent << "if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)\n";
     os << indent << indent << "message(FATAL_ERROR \"ABSOLUTE path INSTALL "
        << "DESTINATION forbidden (by caller): "
        << "${CMAKE_ABSOLUTE_DESTINATION_FILES}\")\n";
     os << indent << "endif()\n";
     }
  os << "file(INSTALL DESTINATION \"" << dest << "\" TYPE " << stype;
  if(optional)
    {
    os << " OPTIONAL";
    }
  switch(this->Message)
    {
    case MessageDefault: break;
    case MessageAlways: os << " MESSAGE_ALWAYS"; break;
    case MessageLazy:   os << " MESSAGE_LAZY"; break;
    case MessageNever:  os << " MESSAGE_NEVER"; break;
    }
  if(!permissions_file.empty())
    {
    os << " PERMISSIONS" << permissions_file;
    }
  if(!permissions_dir.empty())
    {
    os << " DIR_PERMISSIONS" << permissions_dir;
    }
  if(!rename.empty())
    {
    os << " RENAME \"" << rename << "\"";
    }
  os << " FILES";
  if(files.size() == 1)
    {
    os << " \"" << files[0] << "\"";
    }
  else
    {
    for(std::vector<std::string>::const_iterator fi = files.begin();
        fi != files.end(); ++fi)
      {
      os << "\n" << indent << "  \"" << *fi << "\"";
      }
    os << "\n" << indent << " ";
    if(literal_args.empty())
      {
      os << " ";
      }
    }
  if(!literal_args.empty())
    {
    os << literal_args;
    }
  os << ")\n";
}

//----------------------------------------------------------------------------
std::string
cmInstallGenerator::CreateComponentTest(const std::string& component)
{
  std::string result = "NOT CMAKE_INSTALL_COMPONENT OR "
    "\"${CMAKE_INSTALL_COMPONENT}\" STREQUAL \"";
  result += component;
  result += "\"";
  return result;
}

//----------------------------------------------------------------------------
void cmInstallGenerator::GenerateScript(std::ostream& os)
{
  // Track indentation.
  Indent indent;

  // Begin this block of installation.
  std::string component_test =
    this->CreateComponentTest(this->Component);
  os << indent << "if(" << component_test << ")\n";

  // Generate the script possibly with per-configuration code.
  this->GenerateScriptConfigs(os, indent.Next());

  // End this block of installation.
  os << indent << "endif()\n\n";
}

//----------------------------------------------------------------------------
bool cmInstallGenerator::InstallsForConfig(const std::string& config)
{
  return this->GeneratesForConfig(config);
}

//----------------------------------------------------------------------------
std::string cmInstallGenerator::GetInstallDestination() const
{
  std::string result;
  if(!this->Destination.empty() &&
     !cmSystemTools::FileIsFullPath(this->Destination))
    {
    result = "${CMAKE_INSTALL_PREFIX}/";
    }
  result += this->Destination;
  return result;
}

//----------------------------------------------------------------------------
cmInstallGenerator::MessageLevel
cmInstallGenerator::SelectMessageLevel(cmMakefile* mf, bool never)
{
  if(never)
    {
    return MessageNever;
    }
  std::string m = mf->GetSafeDefinition("CMAKE_INSTALL_MESSAGE");
  if(m == "ALWAYS")
    {
    return MessageAlways;
    }
  if(m == "LAZY")
    {
    return MessageLazy;
    }
  if(m == "NEVER")
    {
    return MessageNever;
    }
  return MessageDefault;
}
