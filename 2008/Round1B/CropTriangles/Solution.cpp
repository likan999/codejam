#include <cstdint>

#include "Runner/Runner.h"

using namespace std;

int64_t compute(int n, int A, int B, int C, int D, int x0, int y0, int M) {
  int64_t counts[3][3] = { };
  int64_t x = x0, y = y0;
  for (int i = 0; i < n; i++) {
    ++counts[x % 3][y % 3];
    x = (A * x + B) % M;
    y = (C * y + D) % M;
  }
  int64_t total = 0;
  for (int i = 0; i < 9; i++) {
    int x1 = i / 3;
    int y1 = i % 3;
    int64_t count1 = counts[x1][y1];
    for (int j = 0; j < 9; j++) {
      if (j == i) {
        total += count1 * (count1 - 1) * (count1 - 2);
      } else {
        int x2 = j / 3;
        int y2 = j % 3;
        int x3 = (6 - x1 - x2) % 3;
        int y3 = (6 - y1 - y2) % 3;
        total += count1 * counts[x2][y2] * counts[x3][y3];
      }
    }
  }
  return total / 6;
}

std::unique_ptr<Runner> Runner::instance(new FunctionRunner(&compute));
