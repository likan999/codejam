cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

# Script directory.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/Scripts)

# Include necessary utility scripts.
include(CheckCXX11)
include(Utils)

# Enable LTO
set_property(GLOBAL PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE 1)
set_property(GLOBAL PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO 1)

# Find Boost library.
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS
  program_options
)

# Find glog library.
find_non_standard_package(Folly REQUIRED)

# Find glog library.
find_non_standard_package(Glog REQUIRED)

# Find gtest library.
find_package(GTest REQUIRED)

# Add common include directories.
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(${Folly_INCLUDE_DIRS})
include_directories(${Glog_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})

# Link to those common libraries.
macro(add_solver_executable NAME)
  # invoke built-in add_executable
  add_executable(${ARGV})
  if (TARGET ${NAME})
    get_target_property(__tmp_IS_TARGET_IMPORTED ${NAME} IMPORTED)
    if (NOT ${__tmp_IS_TARGET_IMPORTED})
      # Make the executable name shorter.
      split_string(${NAME} "-" __tmp_SPLITTED_NAME)
      list(GET __tmp_SPLITTED_NAME -1 __tmp_EXECUTABLE_NAME)
      set_target_properties(${NAME} PROPERTIES OUTPUT_NAME ${__tmp_EXECUTABLE_NAME})
      # Link to necessary libraries.
      target_link_libraries(${NAME} Runner)
      target_link_libraries(${NAME} Runner-Main)
    endif()
  endif()
endmacro()

# Test by running and comparing results.
macro(add_solver_test TARGET NAME INPUT OUTPUT SPECS)
  add_test(NAME Test-${TARGET}-${NAME}-Run
    COMMAND $<TARGET_FILE:${TARGET}> --input ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT} --output ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT})
  add_test(NAME Test-${TARGET}-${NAME}-Compare
    COMMAND $<TARGET_FILE:OutputCompareTest> --expected ${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT} --actual ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT} --specs ${SPECS})
endmacro()

# Add the top of source tree as the include path.
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})

enable_testing()

add_subdirectory(Common)
add_subdirectory(Runner)
add_subdirectory(Testing)
add_subdirectory(2008)
add_subdirectory(2013)
add_subdirectory(2014)
