cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(2008/QualificationRound/TrainTimetable)

add_solver_executable(2008-QualificationRound-TrainTimetable Solution.cpp)
target_link_libraries(2008-QualificationRound-TrainTimetable Io)
set_target_properties(2008-QualificationRound-TrainTimetable PROPERTIES OUTPUT_NAME TrainTimetable)
