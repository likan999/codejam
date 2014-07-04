cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(2008/Round1A/Numbers)

add_solver_executable(2008-Round1A-Numbers Solution.cpp)
set_target_properties(2008-Round1A-Numbers PROPERTIES OUTPUT_NAME Numbers)

add_solver_test(2008-Round1A-Numbers Example example.in example.out string:whole-line)
add_solver_test(2008-Round1A-Numbers Small C-small-practice.in C-small-practice.out string:whole-line)
add_solver_test(2008-Round1A-Numbers Large C-large-practice.in C-large-practice.out string:whole-line)
