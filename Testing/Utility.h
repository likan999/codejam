#pragma once

#include <string>
#include <vector>

#include "gtest/gtest.h"

// Utility functions.
std::vector<std::string> tokenize(const std::string& s, char sep);

// GTest predicates.
::testing::AssertionResult isPrefix(const std::string& prefix, const std::string& s);
::testing::AssertionResult isApproximatelyEqual(double expected, double actual, double epsilon);
