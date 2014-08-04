/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2000-2009 Kitware, Inc.

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/

#ifndef cmCTestBuildHandler_h
#define cmCTestBuildHandler_h


#include "cmCTestGenericHandler.h"
#include "cmListFileCache.h"

#include <cmsys/RegularExpression.hxx>

class cmMakefile;

/** \class cmCTestBuildHandler
 * \brief A class that handles ctest -S invocations
 *
 */
class cmCTestBuildHandler : public cmCTestGenericHandler
{
public:
  cmTypeMacro(cmCTestBuildHandler, cmCTestGenericHandler);

  /*
   * The main entry point for this class
   */
  int ProcessHandler();

  cmCTestBuildHandler();

  void PopulateCustomVectors(cmMakefile *mf);

  /**
   * Initialize handler
   */
  virtual void Initialize();

  int GetTotalErrors() { return this->TotalErrors;}
  int GetTotalWarnings() { return this->TotalWarnings;}

private:
  std::string GetMakeCommand();

  //! Run command specialized for make and configure. Returns process status
  // and retVal is return value or exception.
  int RunMakeCommand(const std::string& command,
    int* retVal, const std::string& dir, int timeout,
    std::ostream& ofs);

  enum {
    b_REGULAR_LINE,
    b_WARNING_LINE,
    b_ERROR_LINE
  };

  class cmCTestCompileErrorWarningRex
    {
  public:
    cmCTestCompileErrorWarningRex() {}
    int FileIndex;
    int LineIndex;
    cmsys::RegularExpression RegularExpression;
    };

  struct cmCTestBuildErrorWarning
  {
    bool        Error;
    int         LogLine;
    std::string Text;
    std::string SourceFile;
    std::string SourceFileTail;
    int         LineNumber;
    std::string PreContext;
    std::string PostContext;
  };

  // generate the XML output
  void GenerateXMLHeader(std::ostream& os);
  void GenerateXMLLaunched(std::ostream& os);
  void GenerateXMLLogScraped(std::ostream& os);
  void GenerateXMLFooter(std::ostream& os, double elapsed_build_time);
  void GenerateXMLLaunchedFragment(std::ostream& os, const std::string& fname);
  bool IsLaunchedErrorFile(const std::string& fname);
  bool IsLaunchedWarningFile(const std::string& fname);

  std::string             StartBuild;
  std::string             EndBuild;
  double                  StartBuildTime;
  double                  EndBuildTime;

  std::vector<std::string> CustomErrorMatches;
  std::vector<std::string> CustomErrorExceptions;
  std::vector<std::string> CustomWarningMatches;
  std::vector<std::string> CustomWarningExceptions;
  std::vector<std::string> ReallyCustomWarningMatches;
  std::vector<std::string> ReallyCustomWarningExceptions;
  std::vector<cmCTestCompileErrorWarningRex> ErrorWarningFileLineRegex;

  std::vector<cmsys::RegularExpression> ErrorMatchRegex;
  std::vector<cmsys::RegularExpression> ErrorExceptionRegex;
  std::vector<cmsys::RegularExpression> WarningMatchRegex;
  std::vector<cmsys::RegularExpression> WarningExceptionRegex;

  typedef std::deque<char> t_BuildProcessingQueueType;

  void ProcessBuffer(const char* data, int length, size_t& tick,
    size_t tick_len, std::ostream& ofs, t_BuildProcessingQueueType* queue);
  int ProcessSingleLine(const char* data);

  t_BuildProcessingQueueType            BuildProcessingQueue;
  t_BuildProcessingQueueType            BuildProcessingErrorQueue;
  size_t                                BuildOutputLogSize;
  std::vector<char>                     CurrentProcessingLine;

  std::string                           SimplifySourceDir;
  std::string                           SimplifyBuildDir;
  size_t                                OutputLineCounter;
  typedef std::vector<cmCTestBuildErrorWarning> t_ErrorsAndWarningsVector;
  t_ErrorsAndWarningsVector             ErrorsAndWarnings;
  t_ErrorsAndWarningsVector::iterator   LastErrorOrWarning;
  size_t                                PostContextCount;
  size_t                                MaxPreContext;
  size_t                                MaxPostContext;
  std::deque<std::string>               PreContext;

  int                                   TotalErrors;
  int                                   TotalWarnings;
  char                                  LastTickChar;

  bool                                  ErrorQuotaReached;
  bool                                  WarningQuotaReached;

  int                                   MaxErrors;
  int                                   MaxWarnings;

  bool UseCTestLaunch;
  std::string CTestLaunchDir;
  class LaunchHelper;
  friend class LaunchHelper;
  class FragmentCompare;
};

#endif
