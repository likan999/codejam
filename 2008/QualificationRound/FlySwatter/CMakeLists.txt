cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(2008/QualificationRound/FlySwatter)

add_solver_executable(2008-QualificationRound-FlySwatter Solution.cpp)
set_target_properties(2008-QualificationRound-FlySwatter PROPERTIES OUTPUT_NAME FlySwatter)
