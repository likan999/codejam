#include "Utility.h"

#include <cmath>
#include <string>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

using namespace std;

// Some utility functions.
vector<string> tokenize(const string& s, char sep) {
  vector<string> tokens;
  size_t pos = 0;
  do {
    size_t next = s.find(sep, pos);
    string token;
    if (next == string::npos) {
      token = s.substr(pos);
      pos = string::npos;
    } else {
      token = s.substr(pos, next - pos);
      pos = next + 1;
    }
    if (!token.empty()) {
      tokens.emplace_back(move(token));
    }
  } while (pos != string::npos);
  return tokens;
}

// Some gtest predicates.
::testing::AssertionResult isPrefix(const std::string& prefix, const std::string& s) {
  if (s.substr(0, prefix.length()) != prefix) {
    return ::testing::AssertionFailure() << "`" << prefix << "` is not prefix of `" << s << "`";
  } else {
    return ::testing::AssertionSuccess();
  }
}

::testing::AssertionResult isApproximatelyEqual(double expected, double actual, double epsilon) {
  double delta = abs(expected - actual);
  if (delta <= epsilon) {
    return ::testing::AssertionSuccess();
  }
  if (abs(expected) <= epsilon || abs(actual) <= epsilon) {
    return ::testing::AssertionFailure() << "either " << expected << " or " << actual << " is too close to zero(" << epsilon << ")";
  }
  if (abs(delta / expected) <= epsilon || abs(delta/actual) <= epsilon) {
    return ::testing::AssertionSuccess();
  }
  return ::testing::AssertionFailure() << expected << " and " << actual << " have relative difference more than " << epsilon;
}

