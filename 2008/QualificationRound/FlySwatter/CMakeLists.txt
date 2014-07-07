cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(2008/QualificationRound/FlySwatter)

add_solver_executable(2008-QualificationRound-FlySwatter Solution.cpp)

add_solver_test(2008-QualificationRound-FlySwatter Example example.in example.out double)
add_solver_test(2008-QualificationRound-FlySwatter Small C-small-practice.in C-small-practice.out double)
add_solver_test(2008-QualificationRound-FlySwatter Large C-large-practice.in C-large-practice.out double)
