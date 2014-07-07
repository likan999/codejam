#include "Comparator.h"

#include <iostream>
#include <string>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>

#include "folly/String.h"
#include "gtest/gtest.h"

#include "Testing/Utility.h"

using namespace std;

namespace po = boost::program_options;

namespace {

void parseCommandLine(const char* arg0, const string& spec, const po::options_description& desc, po::variables_map& vm) {
  vector<string> tokens;
  folly::split(':', spec, tokens, true);
  vector<const char*> args = { arg0 };
  for (auto& token: tokens) {
    token = "--" + token;
    args.emplace_back(token.c_str());
  }
  po::store(po::parse_command_line(args.size(), args.data(), desc), vm);
  po::notify(vm);
}

} // namespace anonymous

TypedComparator<double>::TypedComparator(const string& spec) {
  po::options_description desc;
  desc.add_options()
    ("eps", po::value<double>(&epsilon_)->default_value(1e-6), "epsilon")
  ;
  po::variables_map vm;
  parseCommandLine("TypedComparator<double>", spec, desc, vm);
}

void TypedComparator<double>::readAndCompare(istream& expectedStream, istream& actualStream) {
  double expected, actual;
  expectedStream >> expected;
  actualStream >> actual;
  EXPECT_TRUE(isApproximatelyEqual(expected, actual, epsilon_));
}

TypedComparator<string>::TypedComparator(const string& spec) {
  po::options_description desc;
  desc.add_options()
    ("whole-line", po::bool_switch(&wholeLine_)->default_value(false), "read the whole line")
  ;
  po::variables_map vm;
  parseCommandLine("TypedComparator<string>", spec, desc, vm);
}

void TypedComparator<string>::readAndCompare(istream& expectedStream, istream& actualStream) {
  string expected, actual;
  if (wholeLine_) {
    ASSERT_TRUE(getline(expectedStream, expected));
    ASSERT_TRUE(getline(actualStream, actual));
  } else {
    expectedStream >> expected;
    actualStream >> actual;
  }
  EXPECT_EQ(expected, actual);
}

