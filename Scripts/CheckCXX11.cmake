cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

include(CheckCXXCompilerFlag)

# Requires C++11.
CHECK_CXX_COMPILER_FLAG("-std=c++0x" STD_CXX0X_FLAGS_SUPPORTED)
if (STD_CXX0X_FLAGS_SUPPORTED)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message(FATAL_ERROR "The compiler doesn't support -std=c++0x compiler flag")
endif()
