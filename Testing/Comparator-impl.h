#pragma once

#include <iostream>
#include <string>

#include "glog/logging.h"
#include "gtest/gtest.h"

template <typename T>
TypedComparator<T>::TypedComparator(const std::string& spec) {
  CHECK(spec.empty()) << "spec `" << spec << "` should be empty";
}

template <typename T>
void TypedComparator<T>::readAndCompare(std::istream& expectedStream, std::istream& actualStream) {
  T expected, actual;
  expectedStream >> expected;
  actualStream >> actual;
  EXPECT_EQ(expected, actual);
}

