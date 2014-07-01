#include <cmath>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>

#include "glog/logging.h"
#include "gtest/gtest.h"

using namespace std;

namespace po = boost::program_options;

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
      tokens.emplace_back(token);
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

// Command line options.
struct Options {
  string expectedFile;
  string actualFile;
  vector<string> specs;
};

Options ParseCommandLineArguments(int argc, char** argv) {
  Options options;

  string specs;
  po::options_description desc;
  desc.add_options()
    ("help", "print this help message")
    ("expected,e", po::value<string>(&options.expectedFile)->required(), "the expected output file")
    ("actual,a", po::value<string>(&options.actualFile)->required(), "the actual output file")
    ("specs,s", po::value<string>(&specs)->required(), "the specification on how to interpret the output")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);

  // Check help before notify to avoid error raised on validation.
  if (vm.count("help")) {
    cerr << desc << endl;
    exit(1);
  }

  po::notify(vm);

  options.specs = tokenize(specs, ',');

  return options;
}

// Define comparators.
class Comparator {
 public:
  virtual ~Comparator() = default;
  virtual void readAndCompare(istream& expectedStream, istream& actualStream) = 0;
};

template <typename T>
class TypedComparator : public Comparator {
 public:
  TypedComparator(const string& spec) {
    CHECK(spec.empty()) << "spec `" << spec << "` should be empty";
  }

  virtual void readAndCompare(istream& expectedStream, istream& actualStream) override {
    T expected, actual;
    expectedStream >> expected;
    actualStream >> actual;
    EXPECT_EQ(expected, actual);
  }
};

template <>
class TypedComparator<double> : public Comparator {
 public:
  TypedComparator(const string& spec) {
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

  virtual void readAndCompare(istream& expectedStream, istream& actualStream) override {
    double expected, actual;
    expectedStream >> expected;
    actualStream >> actual;
    EXPECT_TRUE(isApproximatelyEqual(expected, actual, epsilon_));
  }

 private:
  double epsilon_;
};

const static unordered_map<string, function<Comparator*(const string& spec)>> comparatorMap = {
  {"int", [](const string& spec) { return new TypedComparator<int>(spec); }},
  {"double", [](const string& spec) { return new TypedComparator<double>(spec); }},
};

vector<unique_ptr<Comparator>> buildComparators(const vector<string>& specs) {
  vector<unique_ptr<Comparator>> comparators;
  for (const auto& spec: specs) {
    auto pos = spec.find(':');
    string header, specWithoutHeader;
    if (pos == string::npos) {
      header = spec;
      specWithoutHeader = "";
    } else {
      header = spec.substr(0, pos);
      specWithoutHeader = spec.substr(pos + 1);
    }
    auto it = comparatorMap.find(header);
    CHECK(it != comparatorMap.end()) << "Spec `" << spec << "` has unrecognized header `" << header << "`";
    comparators.emplace_back(it->second(specWithoutHeader));
  }
  return comparators;
}

static ifstream expectedStream;
static ifstream actualStream;
static vector<unique_ptr<Comparator>> comparators;

TEST(CompareOutput, All) {
  vector<string> expectedLines;
  vector<string> actualLines;
  string line;
  while (getline(expectedStream, line)) {
    expectedLines.emplace_back(line);
  }
  while (getline(actualStream, line)) {
    actualLines.emplace_back(line);
  }
  EXPECT_EQ(expectedLines.size(), actualLines.size());
  size_t numLines = expectedLines.size();
  for (size_t i = 0; i < numLines; i++) {
    const string& expectedLine = expectedLines[i];
    const string& actualLine = actualLines[i];
    ostringstream prefixStream;
    prefixStream << "Case #" << (i + 1) << ": ";
    const string& prefix = prefixStream.str();
    ASSERT_TRUE(isPrefix(prefix, expectedLine));
    ASSERT_TRUE(isPrefix(prefix, actualLine));
    istringstream expectedStream(expectedLine.substr(prefix.size()));
    istringstream actualStream(actualLine.substr(prefix.size()));
    for (const auto& comparator: comparators) {
      comparator->readAndCompare(expectedStream, actualStream);
    }
  }
}

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  ::testing::InitGoogleTest(&argc, argv);

  Options options = ParseCommandLineArguments(argc, argv);
  comparators = buildComparators(options.specs);
  expectedStream.open(options.expectedFile);
  actualStream.open(options.actualFile);

  return RUN_ALL_TESTS();
}
