#include <algorithm>
#include <iostream>
#include <memory>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

#include "Common/Executor.h"
#include "Common/Io.h"

int compute(io::StringOccupiesWholeLine<>, vector<string> engines, vector<string> queries) {
  const size_t npos = static_cast<size_t>(-1);
  size_t s = engines.size(), q = queries.size();
  if (q == 0) {
    return 0;
  }
  unordered_map<string, size_t> engineToIndex;
  for (size_t i = 0; i < s; i++) {
    engineToIndex[engines[i]] = i;
  }
  vector<size_t> queryIndices;
  for (const auto& query: queries) {
    auto result = engineToIndex.find(query);
    queryIndices.emplace_back(result != engineToIndex.end() ? result->second : npos);
  }
  vector<int> minimumSwitches(s);
  for (size_t i = q - 1; i >= 1; i--) {
    size_t currentQuery = queryIndices[i];
    if (currentQuery != npos) {
      int minimumSwitch = numeric_limits<int>::max();
      for (size_t j = 0; j < s; j++) {
        if (j != currentQuery) {
          minimumSwitch = min(minimumSwitch, minimumSwitches[j]);
        }
      }
      minimumSwitches[currentQuery] = 1 + minimumSwitch;
    }
  }
  size_t firstQuery = queryIndices[0];
  int minimumSwitch = numeric_limits<int>::max();
  for (size_t i = 0; i < s; i++) {
    if (i != firstQuery) {
      minimumSwitch = min(minimumSwitch, minimumSwitches[i]);
    }
  }
  return minimumSwitch;
}

std::unique_ptr<Executor> Executor::instance(new FunctionalExecutor(&compute));
