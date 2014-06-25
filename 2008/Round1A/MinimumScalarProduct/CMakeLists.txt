cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(2008/Round1A/MinimumScalarProduct)

add_solver_executable(2008-Round1A-MinimumScalarProduct Solution.cpp)
target_link_libraries(2008-Round1A-MinimumScalarProduct Io)
set_target_properties(2008-Round1A-MinimumScalarProduct PROPERTIES OUTPUT_NAME MinimumScalarProduct)
