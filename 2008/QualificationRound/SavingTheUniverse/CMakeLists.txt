cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(2008/QualificationRound/SavingTheUniverse)

add_solver_executable(2008-QualificationRound-SavingTheUniverse Solution.cpp)
target_link_libraries(2008-QualificationRound-SavingTheUniverse Runner-Io)

add_solver_test(2008-QualificationRound-SavingTheUniverse Example example.in example.out int)
add_solver_test(2008-QualificationRound-SavingTheUniverse Small A-small-practice.in A-small-practice.out int)
add_solver_test(2008-QualificationRound-SavingTheUniverse Large A-large-practice.in A-large-practice.out int)
