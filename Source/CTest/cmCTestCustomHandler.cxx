/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2000-2009 Kitware, Inc., Insight Software Consortium

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/

#include "cmCTestCustomHandler.h"
#include "cmXMLParser.h"
#include "cmCTest.h"
#include "cmake.h"
#include "cmGeneratedFileStream.h"
#include <cmsys/Process.h>
#include <cmsys/RegularExpression.hxx>
#include <cmsys/Base64.h>
#include <cmsys/FStream.hxx>
#include "cmMakefile.h"
#include "cmXMLSafe.h"

#include <stdlib.h>
#include <math.h>
#include <float.h>

//----------------------------------------------------------------------
cmCTestCustomHandler::cmCTestCustomHandler(std::string const& name)
{
  this->Custom = true;
  this->RunnerName = name;
  this->CustomMaximumPassedTestOutputSize = 0;
  this->CustomMaximumFailedTestOutputSize = 0;
}

//----------------------------------------------------------------------
void cmCTestCustomHandler::Initialize()
{
  this->Superclass::Initialize();
  this->CustomMaximumPassedTestOutputSize = 0;
  this->CustomMaximumFailedTestOutputSize = 0;
}

//----------------------------------------------------------------------
int cmCTestCustomHandler::PreProcessHandler()
{
  if ( !this->InitializeCustomHandler() )
    {
    return 0;
    }

  return 1;
}

//----------------------------------------------------------------------
int cmCTestCustomHandler::PostProcessHandler()
{
  return 1;
}

//----------------------------------------------------------------------
void cmCTestCustomHandler::GenerateTestCommand(
  std::vector<std::string>& args, int test)
{
  std::vector<std::string>::size_type pp;
  cmOStringStream stream;
  std::string customcommand
    = cmSystemTools::ConvertToOutputPath(this->Runner.c_str());
  stream << test;
  for ( pp = 0; pp < this->Options.size(); pp ++ )
    {
    std::string arg = this->Options[pp];
    cmSystemTools::ReplaceString(arg, "??", stream.str().c_str());
    args.push_back(arg);
    customcommand += " \"";
    customcommand += arg;
    customcommand += "\"";
    }
  cmCTestLog(this->CTest, HANDLER_VERBOSE_OUTPUT, "Custom " << this->RunnerName
    << " command: " << customcommand << std::endl);
}

//----------------------------------------------------------------------
bool cmCTestCustomHandler::InitializeCustomHandler()
{
  // Setup the command
  this->Runner = this->CTest->GetCTestConfiguration(
      "CustomRunner" + this->RunnerName + "Command");
  if ( !cmSystemTools::FileExists(this->Runner.c_str()) )
    {
    cmCTestLog(this->CTest, WARNING,
      "Custom runner (CustomRunner" + this->RunnerName + "Command) "
      "not set, or cannot find the specified program."
      << std::endl);
    return false;
    }

  // Setup the options
  std::string args = this->CTest->GetCTestConfiguration(
      "CustomRunner" + this->RunnerName + "Arguments");
  this->Options = cmSystemTools::ParseArguments(args.c_str());

  std::string files = this->CTest->GetCTestConfiguration(
      "CustomRunner" + this->RunnerName + "OutputFiles");
  this->OutputFiles = cmSystemTools::ParseArguments(files.c_str());

  return true;
}

void
cmCTestCustomHandler::appendCustomOutput(cmCTestTestResult& res,
                                         int test)
{
  std::vector<std::string> files = testOutputFileNames(test);

  std::vector<std::string>::const_iterator i;
  for (i = files.begin(); i != files.end(); ++i)
    {
    cmsys::ifstream ifs(i->c_str());
    if ( !ifs )
      {
      std::string log = "Cannot read custom output file: " + *i;
      cmCTestLog(this->CTest, ERROR_MESSAGE, log.c_str() << std::endl);
      continue;
      }
    std::string line;
    while ( cmSystemTools::GetLineFromStream(ifs, line) )
      {
      res.Output += line;
      res.Output += "\n";
      }
    }
}

std::vector<std::string>
cmCTestCustomHandler::testOutputFileNames(int test)
{
  std::vector<std::string> files;
  cmOStringStream stream;
  stream << test;

  std::vector<std::string>::const_iterator i;
  for (i = this->OutputFiles.begin();
       i != this->OutputFiles.end(); ++i)
    {
      std::string file = *i;
      cmSystemTools::ReplaceString(file, "??", stream.str().c_str());

      if ( !cmSystemTools::FileExists(file.c_str()) )
        {
        std::string log = "Cannot find custom output file: " + file;
        cmCTestLog(this->CTest, ERROR_MESSAGE, log.c_str() << std::endl);
        }
      else
        {
        files.push_back(file);
        }
    }

  return files;
}
