#include "Comparator.h"

#include <iostream>
#include <string>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>

#include "gtest/gtest.h"

#include "Testing/Utility.h"

using namespace std;

namespace po = boost::program_options;

TypedComparator<double>::TypedComparator(const string& spec) {
  auto tokens = tokenize(spec, ':');
  vector<const char*> args = { "TypedComparator<double>" };
  for (auto& token: tokens) {
    token = "--" + token;
    args.emplace_back(token.c_str());
  }

  po::options_description desc;
  desc.add_options()
    ("eps", po::value<double>(&epsilon_)->default_value(1e-6), "epsilon")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(args.size(), args.data(), desc), vm);
  po::notify(vm);
}

void TypedComparator<double>::readAndCompare(istream& expectedStream, istream& actualStream) {
  double expected, actual;
  expectedStream >> expected;
  actualStream >> actual;
  EXPECT_TRUE(isApproximatelyEqual(expected, actual, epsilon_));
}

TypedComparator<string>::TypedComparator(const string& spec) {
  auto tokens = tokenize(spec, ':');
  vector<const char*> args = { "TypedComparator<string>" };
  for (auto& token: tokens) {
    token = "--" + token;
    args.emplace_back(token.c_str());
  }

  po::options_description desc;
  desc.add_options()
    ("whole-line", po::bool_switch(&wholeLine_)->default_value(false), "read the whole line")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(args.size(), args.data(), desc), vm);
  po::notify(vm);
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
