cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

# This library tries to find the Google Logging Library
# Right now it is very simple and only tested for a very small set of systems.
#
# This is file is inspired by FindBoost that comes with CMake installation.
#
# This module defines
#  Glog_FOUND
#  Glog_INCLUDE_DIRS
#  Glog_LIBRARIES

if (NOT Glog_INCLUDE_DIR)
  find_path(Glog_INCLUDE_DIR NAMES glog/logging.h)
endif()

if (NOT Glog_LIBRARY)
  if (Glog_USE_STATIC_LIBS)
    set(__orig_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
  endif()

  find_library(Glog_LIBRARY NAMES glog)

  if (Glog_USE_STATIC_LIBS)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${__orig_CMAKE_FIND_LIBRARY_SUFFIXES})
  endif()
endif()

set(Glog_INCLUDE_DIRS ${Glog_INCLUDE_DIR})
set(Glog_LIBRARIES ${Glog_LIBRARY})

mark_as_advanced(
  Glog_INCLUDE_DIR
  Glog_INCLUDE_DIRS
  Glog_LIBRARY
  Glog_LIBRARIES
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Glog
  DEFAULT_MSG
  Glog_INCLUDE_DIRS
  Glog_LIBRARIES
)
