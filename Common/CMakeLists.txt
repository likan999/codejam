cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(Common)

add_library(Main Main.cpp Executor.cpp)
target_link_libraries(Main ${Glog_LIBRARIES})
