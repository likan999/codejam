cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(2013/Round1B/FallingDiamonds)

add_solver_executable(2013-Round1B-FallingDiamonds Solution.cpp)
set_target_properties(2013-Round1B-FallingDiamonds PROPERTIES OUTPUT_NAME FallingDiamonds)

add_solver_test(2013-Round1B-FallingDiamonds Example example.in example.out double)
add_solver_test(2013-Round1B-FallingDiamonds Small B-small-practice.in B-small-practice.out double)
add_solver_test(2013-Round1B-FallingDiamonds Large B-large-practice.in B-large-practice.out double)
