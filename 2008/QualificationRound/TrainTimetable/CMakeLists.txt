cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(2008/QualificationRound/TrainTimetable)

add_solver_executable(2008-QualificationRound-TrainTimetable Solution.cpp)
target_link_libraries(2008-QualificationRound-TrainTimetable Runner-Io)

add_solver_test(2008-QualificationRound-TrainTimetable Example example.in example.out int,int)
add_solver_test(2008-QualificationRound-TrainTimetable Small B-small-practice.in B-small-practice.out int,int)
add_solver_test(2008-QualificationRound-TrainTimetable Large B-large-practice.in B-large-practice.out int,int)
