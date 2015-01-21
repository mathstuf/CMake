" =============================================================================
"
"   Program:   CMake - Cross-Platform Makefile Generator
"   Module:    $RCSfile$
"   Language:  VIM
"   Date:      $Date$
"   Version:   $Revision$
"
" =============================================================================

" Vim syntax file
" Language:     CMake
" Author:       Andy Cedilnik <andy.cedilnik@kitware.com>
" Maintainer:   Karthik Krishnan <karthik.krishnan@kitware.com>
" Last Change:  $Date$
" Version:      $Revision$
"
" Licence:      The CMake license applies to this file. See
"               http://www.cmake.org/licensing
"               This implies that distribution with Vim is allowed

if exists("b:current_syntax")
  finish
endif

syn match cmakeEscaped /\(\\\\\|\\"\|\\n\|\\t\)/ contained
syn region cmakeComment start="#" end="$" contains=cmakeTodo,@Spell
syn region cmakeLuaComment start="\[\z(=*\)\[" end="\]\z1\]" contains=cmakeTodo,@Spell
syn region cmakeRegistry start=/\[/ end=/]/
            \ contained oneline contains=CONTAINED,cmakeTodo,cmakeEscaped
syn region cmakeVariableValue start=/\${/ end=/}/
            \ contained oneline contains=CONTAINED,cmakeTodo
syn region cmakeEnvironment start=/\$ENV{/ end=/}/
            \ contained oneline contains=CONTAINED,cmakeTodo
syn region cmakeString start=/"/ end=/"/
            \ contains=CONTAINED,cmakeTodo,cmakeOperators
syn region cmakeArguments start=/(/ end=/)/
            \ contains=ALLBUT,cmakeArguments,cmakeTodo
syn keyword cmakeSystemVariables
            \ APPLE BORLAND CYGWIN MSVC MSVC10 MSVC11 MSVC12 MSVC14 MSVC60
            \ MSVC70 MSVC71 MSVC80 MSVC90 MSVC_IDE UNIX WIN32 WINCE WINDOWS_PHONE WINDOWS_STORE
syn keyword cmakeOperators
            \ NOT COMMAND POLICY TARGET EXISTS IS_NEWER_THAN IS_DIRECTORY
            \ IS_SYMLINK IS_ABSOLUTE MATCHES LESS GREATER EQUAL STRLESS
            \ STRGREATER STREQUAL VERSION_LESS VERSION_EQUAL VERSION_GREATER
            \ DEFINED AND OR
            \ contained
syn keyword cmakeTodo
            \ TODO FIXME XXX
            \ contained

" The keywords are block-selected from Help/manual/cmake-commands.7.rst.
syn case ignore
syn keyword cmakeStatement
            \ add_compile_options
            \ add_custom_command
            \ add_custom_target
            \ add_definitions
            \ add_dependencies
            \ add_executable
            \ add_library
            \ add_subdirectory
            \ add_test
            \ aux_source_directory
            \ break
            \ build_command
            \ cmake_host_system_information
            \ cmake_minimum_required
            \ cmake_policy
            \ configure_file
            \ continue
            \ create_test_sourcelist
            \ define_property
            \ elseif
            \ else
            \ enable_language
            \ enable_testing
            \ endforeach
            \ endfunction
            \ endif
            \ endmacro
            \ endwhile
            \ execute_process
            \ export
            \ file
            \ find_file
            \ find_library
            \ find_package
            \ find_path
            \ find_program
            \ fltk_wrap_ui
            \ foreach
            \ function
            \ get_cmake_property
            \ get_directory_property
            \ get_filename_component
            \ get_property
            \ get_source_file_property
            \ get_target_property
            \ get_test_property
            \ if
            \ include_directories
            \ include_external_msproject
            \ include_regular_expression
            \ include
            \ install
            \ link_directories
            \ list
            \ load_cache
            \ load_command
            \ macro
            \ mark_as_advanced
            \ math
            \ message
            \ option
            \ project
            \ qt_wrap_cpp
            \ qt_wrap_ui
            \ remove_definitions
            \ return
            \ separate_arguments
            \ set_directory_properties
            \ set_property
            \ set
            \ set_source_files_properties
            \ set_target_properties
            \ set_tests_properties
            \ site_name
            \ source_group
            \ string
            \ target_compile_definitions
            \ target_compile_features
            \ target_compile_options
            \ target_include_directories
            \ target_link_libraries
            \ target_sources
            \ try_compile
            \ try_run
            \ unset
            \ variable_watch
            \ while
            \ contained
            \ nextgroup=cmakeArguments
syn keyword cmakeDeprecated
            \ build_name
            \ exec_program
            \ export_library_dependencies
            \ install_files
            \ install_programs
            \ install_targets
            \ link_libraries
            \ make_directory
            \ output_required_files
            \ remove
            \ subdir_depends
            \ subdirs
            \ use_mangled_mesa
            \ utility_source
            \ variable_requires
            \ write_file
            \ contained
            \ nextgroup=cmakeArguments
syn match cmakeFunction /[a-z0-9_]\+\ze\s*(/
            \ contains=cmakeStatement,cmakeDeprecated
            \ nextgroup=cmakeArguments

" Define the default highlighting.
hi def link cmakeStatement       Keyword
hi def link cmakeDeprecated      WarningMsg
hi def link cmakeFunction        Function
hi def link cmakeComment         Comment
hi def link cmakeLuaComment      Comment
hi def link cmakeString          String
hi def link cmakeSystemVariables Define
hi def link cmakeVariableValue   Constant
hi def link cmakeRegistry        Underlined
hi def link cmakeArguments       Identifier
hi def link cmakeEnvironment     Special
hi def link cmakeOperators       Conditional
hi def link cmakeTodo            TODO
hi def link cmakeEscaped         Special

let b:current_syntax = "cmake"

"EOF"
