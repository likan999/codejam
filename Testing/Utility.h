#pragma once

#include <string>
#include <vector>

#include "gtest/gtest.h"

// GTest predicates.
::testing::AssertionResult isPrefix(const std::string& prefix, const std::string& s);
::testing::AssertionResult isApproximatelyEqual(double expected, double actual, double epsilon);
