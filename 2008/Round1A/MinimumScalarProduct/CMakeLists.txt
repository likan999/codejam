cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(2008/Round1A/MinimumScalarProduct)

add_solver_executable(2008-Round1A-MinimumScalarProduct Solution.cpp)
target_link_libraries(2008-Round1A-MinimumScalarProduct Runner-Io)
set_target_properties(2008-Round1A-MinimumScalarProduct PROPERTIES OUTPUT_NAME MinimumScalarProduct)

add_solver_test(2008-Round1A-MinimumScalarProduct Example example.in example.out int)
add_solver_test(2008-Round1A-MinimumScalarProduct Small A-small-practice.in A-small-practice.out int)
add_solver_test(2008-Round1A-MinimumScalarProduct Large A-large-practice.in A-large-practice.out int)
