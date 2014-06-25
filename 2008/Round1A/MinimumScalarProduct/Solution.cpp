#include <algorithm>
#include <cstdint>
#include <vector>

#include "Common/Executor.h"
#include "Common/Io.h"

using namespace std;

int64_t compute(io::PackedVector<2, 1, 0, 0>, vector<int> u, vector<int> v) {
  size_t n = u.size();
  sort(u.begin(), u.end());
  sort(v.begin(), v.end());
  reverse(v.begin(), v.end());
  int64_t total = 0;
  for (size_t i = 0; i < n; i++) {
    total += static_cast<int64_t>(u[i]) * static_cast<int64_t>(v[i]);
  }
  return total;
}

std::unique_ptr<Executor> Executor::instance(new FunctionalExecutor(&compute));
