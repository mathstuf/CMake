/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2000-2009 Kitware, Inc., Insight Software Consortium

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#ifndef cmInstallGenerator_h
#define cmInstallGenerator_h

#include "cmInstallType.h"
#include "cmScriptGenerator.h"

class cmLocalGenerator;
class cmMakefile;

/** \class cmInstallGenerator
 * \brief Support class for generating install scripts.
 *
 */
class cmInstallGenerator: public cmScriptGenerator
{
public:
  enum MessageLevel
  {
    MessageDefault,
    MessageAlways,
    MessageLazy,
    MessageNever
  };

  cmInstallGenerator(const std::string& destination,
                     std::vector<std::string> const& configurations,
                     const std::string& component,
                     MessageLevel message);
  virtual ~cmInstallGenerator();

  void AddInstallRule(
    std::ostream& os, cmInstallType type,
    std::vector<std::string> const& files,
    bool optional = false,
    const std::string& permissions_file = "",
    const std::string& permissions_dir = "",
    const std::string& rename = "",
    const std::string& literal_args = "",
    Indent const& indent = Indent()
    );

  const std::string& GetDestination() const
    { return this->Destination; }

  /** Get the install destination as it should appear in the
      installation script.  */
  std::string GetInstallDestination() const;

  /** Test if this generator installs something for a given configuration.  */
  bool InstallsForConfig(const std::string& config);

  /** Select message level from CMAKE_INSTALL_MESSAGE or 'never'.  */
  static MessageLevel SelectMessageLevel(cmMakefile* mf, bool never = false);

protected:
  virtual void GenerateScript(std::ostream& os);

  std::string CreateComponentTest(const std::string& component);

  // Information shared by most generator types.
  std::string Destination;
  std::string Component;
  MessageLevel Message;
};

#endif
