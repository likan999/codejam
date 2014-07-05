cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(Runner)

add_library(Runner Runner.cpp)
target_link_libraries(Runner
  ${Glog_LIBRARIES}
  Common-IntegerSequence
  Runner-Io
)

add_library(Runner-Io INTERFACE)
target_link_libraries(Runner-Io INTERFACE
  ${Boost_LIBRARIES}
  ${Glog_LIBRARIES}
  Common-IntegerSequence
  Runner-StreamScanf
)

add_library(Runner-Main Main.cpp)
target_link_libraries(Runner-Main
  ${Boost_LIBRARIES}
  ${Glog_LIBRARIES}
  Runner
)

add_library(Runner-StreamScanf StreamScanf.cpp)
target_link_libraries(Runner-StreamScanf
  ${Glog_LIBRARIES}
)
