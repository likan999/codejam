cmake_minimum_required (VERSION 2.8 FATAL_ERROR)

# This library tries to find the Facebook Folly Library
# Right now it is very simple and only tested for a very small set of systems.
#
# This is file is inspired by FindBoost that comes with CMake installation.
#
# This module defines
#  Folly_FOUND
#  Folly_INCLUDE_DIRS
#  Folly_LIBRARIES

if (NOT Folly_INCLUDE_DIR)
  find_path(Folly_INCLUDE_DIR NAMES folly/folly-config.h)
endif()

if (NOT Folly_LIBRARY)
  if (Folly_USE_STATIC_LIBS)
    set(__orig_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
  endif()

  find_library(Folly_LIBRARY NAMES folly)

  if (Folly_USE_STATIC_LIBS)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${__orig_CMAKE_FIND_LIBRARY_SUFFIXES})
  endif()
endif()

set(Folly_INCLUDE_DIRS ${Folly_INCLUDE_DIR})
set(Folly_LIBRARIES ${Folly_LIBRARY})

mark_as_advanced(
  Folly_INCLUDE_DIR
  Folly_INCLUDE_DIRS
  Folly_LIBRARY
  Folly_LIBRARIES
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Folly
  DEFAULT_MSG
  Folly_INCLUDE_DIRS
  Folly_LIBRARIES
)
