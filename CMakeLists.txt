cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

# Script directory.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/Scripts)

# Enable LTO
set_property(GLOBAL PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE 1)
set_property(GLOBAL PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO 1)

include(CheckCXXCompilerFlag)

# Requires C++11.
CHECK_CXX_COMPILER_FLAG("-std=c++0x" STD_CXX0X_FLAGS_SUPPORTED)
if (STD_CXX0X_FLAGS_SUPPORTED)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message(FATAL_ERROR "The compiler doesn't support -std=c++0x compiler flag")
endif()

# Find glog library.
find_path(Glog_INCLUDE_DIR NAMES glog/logging.h)
find_library(Glog_LIBRARY NAMES glog)
set(Glog_INCLUDE_DIRS ${Glog_INCLUDE_DIR})
set(Glog_LIBRARIES ${Glog_LIBRARY})

# Add common include directories.
include_directories(SYSTEM ${Glog_INCLUDE_DIRS})

# Link to those common libraries.
macro(add_solver_executable NAME)
  # invoke built-in add_executable
  add_executable(${ARGV})
  if (TARGET ${NAME})
    get_target_property(__tmp_IS_TARGET_IMPORTED ${NAME} IMPORTED)
    if (NOT ${__tmp_IS_TARGET_IMPORTED})
      target_link_libraries(${NAME} Main)
    endif()
  endif()
endmacro()

# Add the top of source tree as the include path.
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})

add_subdirectory(Common)
add_subdirectory(2008)
add_subdirectory(2013)
add_subdirectory(2014)
