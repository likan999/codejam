cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(Testing)

add_executable(OutputCompareTest OutputCompareTest.cpp)
target_link_libraries(OutputCompareTest
  ${Boost_LIBRARIES}
  ${Glog_LIBRARIES}
  ${GTEST_LIBRARIES}
  Test-Comparator
  Test-Utility
)

add_library(Test-Comparator Comparator.cpp)
target_link_libraries(Test-Comparator
  ${Boost_LIBRARIES}
  ${GTEST_LIBRARIES}
  Test-Utility
)

add_library(Test-Utility Utility.cpp)
target_link_libraries(Test-Utility
  ${GTEST_LIBRARIES}
)
