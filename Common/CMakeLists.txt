cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(Common)

add_library(Executor Executor.cpp)
target_link_libraries(Executor
  ${Glog_LIBRARIES}
  IntegerSequence
  Io
)

add_library(IntegerSequence INTERFACE)

add_library(Io INTERFACE)
target_link_libraries(Io INTERFACE
  ${Glog_LIBRARIES}
  IntegerSequence
  StreamScanf
)

add_library(Main Main.cpp)
target_link_libraries(Main
  ${Glog_LIBRARIES}
  Executor
)

add_library(StreamScanf StreamScanf.cpp)
target_link_libraries(StreamScanf
  ${Glog_LIBRARIES}
)
