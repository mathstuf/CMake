/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2000-2009 Kitware, Inc.

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/

#ifndef cmCTestCustomHandler_h
#define cmCTestCustomHandler_h


#include "cmCTestTestHandler.h"
#include "cmListFileCache.h"

class cmMakefile;

/** \class cmCTestCustomHandler
 * \brief A class that handles ctest -S invocations
 *
 */
class cmCTestCustomHandler : public cmCTestTestHandler
{
  friend class cmCTestRunTest;
public:
  cmTypeMacro(cmCTestCustomHandler, cmCTestTestHandler);

  cmCTestCustomHandler(std::string const& name);

  void Initialize();
protected:
  virtual int PreProcessHandler();
  virtual int PostProcessHandler();
  virtual void GenerateTestCommand(std::vector<std::string>& args, int test);

private:
  std::string RunnerName;
  std::string Runner;
  std::vector<std::string> Options;
  std::vector<std::string> OutputFiles;

  ///! Initialize custom runner subsystem.
  bool InitializeCustomHandler();

  ///! append OutputFile to the test log
  void appendCustomOutput(cmCTestTestHandler::cmCTestTestResult& res,
                          int test);

  ///! generate the output filenames for the given test index
  std::vector<std::string> testOutputFileNames(int test);
};

#endif
