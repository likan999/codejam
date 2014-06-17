#include <algorithm>
#include <cmath>
#include <memory>

#include "Common/Executor.h"

using namespace std;

int floorSqrt(int n) {
  int s = static_cast<int>(floor(sqrt(n)));
  while (s * s <= n) ++s;
  while (s * s > n) --s;
  return s;
}

double addExp(double x, double y) {
  double normalization = max(x, y);
  double other = min(x, y) - normalization;
  return normalization + log(1 + exp(other));
}

double compute(int n, int x, int y) {
  if (x < 0) {
    x = -x;
  }
  if (x % 2 != y % 2) {
    return 0;
  }
  int k = (floorSqrt(8 * n + 1) + 1)/ 4;
  if (x + y <= 2 * k - 2) {
    return 1;
  } else if (x + y >= 2 * k + 2) {
    return 0;
  } if (x == 0) {
    return 0;
  }
  int r = n - k * (2 * k - 1);
  if (r <= y) {
    return 0;
  } else if (r >= 2 * k + y + 1) {
    return 1;
  }
  double sumExp = 0, lastExp = 0;
  for (int i = 1; i <= y; i++) {
    lastExp += log(static_cast<double>(r - i + 1) / static_cast<double>(i));
    sumExp = addExp(sumExp, lastExp);
  }
  return 1 - exp(sumExp - r * log(2.0));
}

std::unique_ptr<Executor> Executor::instance(new FunctionalExecutor(&compute));
