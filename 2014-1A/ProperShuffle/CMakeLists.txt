cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(2014-1A/ProperShuffle)

add_executable(2014-1A-ProperShuffle Solution.cpp)
set_target_properties(2014-1A-ProperShuffle PROPERTIES OUTPUT_NAME ProperShuffle)

add_executable(2014-1A-ProperShuffle-TestCaseGen TestCaseGen.cpp)
set_target_properties(2014-1A-ProperShuffle-TestCaseGen PROPERTIES OUTPUT_NAME TestCaseGen)
