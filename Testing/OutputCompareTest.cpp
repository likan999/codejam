#include <cstdlib>
#include <fstream>
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>

#include "glog/logging.h"
#include "gtest/gtest.h"

#include "Testing/Comparator.h"
#include "Testing/Utility.h"

using namespace std;

namespace po = boost::program_options;

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

const static unordered_map<string, function<Comparator*(const string& spec)>> comparatorMap = {
  {"int", [](const string& spec) { return new TypedComparator<int>(spec); }},
  {"double", [](const string& spec) { return new TypedComparator<double>(spec); }},
  {"string", [](const string& spec) { return new TypedComparator<string>(spec); }},
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
    expectedLines.emplace_back(move(line));
  }
  while (getline(actualStream, line)) {
    actualLines.emplace_back(move(line));
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
    EXPECT_TRUE(expectedStream.eof());
    EXPECT_TRUE(actualStream.eof());
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
