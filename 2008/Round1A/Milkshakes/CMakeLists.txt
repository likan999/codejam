cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(2008/Round1A/Milkshakes)

add_solver_executable(2008-Round1A-Milkshakes Solution.cpp)
set_target_properties(2008-Round1A-Milkshakes PROPERTIES OUTPUT_NAME Milkshakes)

add_solver_test(2008-Round1A-Milkshakes Example example.in example.out string:whole-line)
add_solver_test(2008-Round1A-Milkshakes Small B-small-practice.in B-small-practice.out string:whole-line)
add_solver_test(2008-Round1A-Milkshakes Large B-large-practice.in B-large-practice.out string:whole-line)
