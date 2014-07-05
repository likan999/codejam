#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>

#include "glog/logging.h"

#include "Runner/Runner.h"

using namespace std;

namespace po = boost::program_options;

static void ParseCommandLineArguments(int argc, char* argv[]) {
  string input;
  string output;

  po::options_description desc;
  desc.add_options()
    ("help", "print this help message")
    ("input,i", po::value<string>(&input), "input file")
    ("output,o", po::value<string>(&output), "output file")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);

  // Check help before notify to avoid error raised on validation.
  if (vm.count("help")) {
    cerr << desc << endl;
    exit(1);
  }

  po::notify(vm);

  if (!input.empty()) {
    CHECK(freopen(input.c_str(), "r", stdin)) << "Failed to open input `" << input << "`: " << strerror(errno);
  }

  if (!output.empty()) {
    CHECK(freopen(output.c_str(), "w", stdout)) << "Failed to open output `" << output << "`: " << strerror(errno);
  }
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  ParseCommandLineArguments(argc, argv);

  Runner::runInstance();
}
