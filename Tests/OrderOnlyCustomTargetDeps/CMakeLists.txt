cmake_minimum_required (VERSION 2.8)
project (OrderOnlyCustomTargetDeps C)

add_custom_target(cmd1
  COMMAND ${CMAKE_COMMAND} -E touch cmd1)
add_custom_target(cmd2
  COMMAND ${CMAKE_COMMAND} -E touch cmd2)

add_library(objlib OBJECT test.c)
add_executable(exec $<TARGET_OBJECTS:objlib>)

add_dependencies(objlib cmd1)
add_dependencies(exec cmd2)
