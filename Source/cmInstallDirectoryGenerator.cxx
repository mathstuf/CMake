/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2000-2009 Kitware, Inc., Insight Software Consortium

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#include "cmInstallDirectoryGenerator.h"

#include "cmTarget.h"

//----------------------------------------------------------------------------
cmInstallDirectoryGenerator
::cmInstallDirectoryGenerator(std::vector<std::string> const& dirs,
                              const std::string& dest,
                              const std::string& file_permissions,
                              const std::string& dir_permissions,
                              std::vector<std::string> const& configurations,
                              const std::string& component,
                              MessageLevel message,
                              const std::string& literal_args,
                              bool optional):
  cmInstallGenerator(dest, configurations, component, message),
  Directories(dirs),
  FilePermissions(file_permissions), DirPermissions(dir_permissions),
  LiteralArguments(literal_args), Optional(optional)
{
}

//----------------------------------------------------------------------------
cmInstallDirectoryGenerator
::~cmInstallDirectoryGenerator()
{
}

//----------------------------------------------------------------------------
void
cmInstallDirectoryGenerator::GenerateScriptActions(std::ostream& os,
                                                   Indent const& indent)
{
  // Write code to install the directories.
  static const std::string no_rename = "";
  this->AddInstallRule(os, cmInstallType_DIRECTORY,
                       this->Directories,
                       this->Optional,
                       this->FilePermissions,
                       this->DirPermissions,
                       no_rename, this->LiteralArguments,
                       indent);
}
