cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(2013/Round1B/FallingDiamonds)

add_solver_executable(2013-Round1B-FallingDiamonds Solution.cpp)
set_target_properties(2013-Round1B-FallingDiamonds PROPERTIES OUTPUT_NAME FallingDiamonds)
