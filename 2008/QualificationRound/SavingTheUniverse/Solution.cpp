#include <algorithm>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

#include "Runner/Io.h"
#include "Runner/Runner.h"

int compute(io::StringOccupiesWholeLine<>, const vector<string>& engines, const vector<string>& queries) {
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
  int minimumSwitch;
  for (size_t i = q; i > 0; i--) {
    size_t currentQuery = queryIndices[i - 1];
    if (currentQuery != npos || i == 1) {
      minimumSwitch = numeric_limits<int>::max();
      for (size_t j = 0; j < s; j++) {
        if (j != currentQuery) {
          minimumSwitch = min(minimumSwitch, minimumSwitches[j]);
        }
      }
      minimumSwitches[currentQuery] = 1 + minimumSwitch;
    }
  }
  return minimumSwitch;
}

std::unique_ptr<Runner> Runner::instance(new FunctionRunner(&compute));
