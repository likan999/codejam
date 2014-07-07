cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

# Replacement for find_package that prepends variable ${PACKAGE_NAME}_ROOT to
# the beginning of CMAKE_PREFIX_PATH when searching for packages, and restore
# the value after it is done.
macro(find_non_standard_package NAME)
  string(TOUPPER ${NAME}_ROOT __tmp_ROOT_VAR)
  set(__orig_CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
  if (${${__tmp_ROOT_VAR}})
    list(INSERT CMAKE_PREFIX_PATH 0 ${${__tmp_ROOT_VAR}})
  elseif (NOT $ENV{${__tmp_ROOT_VAR}} STREQUAL "")
    list(INSERT CMAKE_PREFIX_PATH 0 $ENV{${__tmp_ROOT_VAR}})
  endif()
  find_package(${ARGV})
  set(CMAKE_PREFIX_PATH ${__orig_CMAKE_PREFIX_PATH})
endmacro()

# Split the string into list.
macro(split_string INPUT SEP OUTPUT)
  string(REPLACE ${SEP} ";" ${OUTPUT} ${INPUT})
endmacro()
