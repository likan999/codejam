#include <cstdio>
#include <string>

#include "Runner/Runner.h"

using namespace std;

struct Matrix {
  int v00;
  int v01;
  int v10;
  int v11;
};

Matrix multiply(const Matrix& left, const Matrix& right) {
  Matrix result;
  result.v00 = (left.v00 * right.v00 + left.v01 * right.v10) % 1000;
  result.v01 = (left.v00 * right.v01 + left.v01 * right.v11) % 1000;
  result.v10 = (left.v10 * right.v00 + left.v11 * right.v10) % 1000;
  result.v11 = (left.v10 * right.v01 + left.v11 * right.v11) % 1000;
  return result;
}

string compute(int n) {
  Matrix m = { 0, 1, -4, 6 };
  Matrix pow = { 1, 0, 0, 1 };
  unsigned int mask = 1 << 31;
  for (; mask != 0 && (n & mask) == 0; mask >>= 1);
  for (; mask != 0; mask >>= 1) {
    pow = multiply(pow, pow);
    if (n & mask) {
      pow = multiply(pow, m);
    }
  }
  int result = (pow.v00 * 2 + pow.v01 * 6 - 1) % 1000;
  if (result < 0) {
    result += 1000;
  }
  char buf[4];
  sprintf(buf, "%03d", result);
  return buf;
}

std::unique_ptr<Runner> Runner::instance(new FunctionRunner(&compute));
