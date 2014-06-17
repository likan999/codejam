cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(2014/Round1A/ProperShuffle)

add_solver_executable(2014-Round1A-ProperShuffle Solution.cpp)
target_link_libraries(2014-Round1A-ProperShuffle ${Glog_LIBRARIES})
set_target_properties(2014-Round1A-ProperShuffle PROPERTIES OUTPUT_NAME ProperShuffle)

add_executable(2014-Round1A-ProperShuffle-TestCaseGen TestCaseGen.cpp)
set_target_properties(2014-Round1A-ProperShuffle-TestCaseGen PROPERTIES OUTPUT_NAME TestCaseGen)
