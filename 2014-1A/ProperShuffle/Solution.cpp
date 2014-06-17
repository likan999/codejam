#include <cmath>
#include <vector>

#include <glog/logging.h>

#include "Common/Common.h"

using namespace std;

vector<vector<double>> P(1000);
double threshold;

double f(double p) {
  return 1000 * p;
}

void initialize() {
  for (int i = 0; i < 1000; i++) {
    P[i].resize(1000);
    P[i][i] = 1;
  }
  for (int i = 0; i < 1000; i++) {
    for (int j = 0; j < 1000; j++) {
      if (j != i) {
        for (int k = 0; k < 1000; k++) {
          P[j][k] = (P[j][k] * 999 + P[i][k]) / 1000;
        }
      }
    }
    for (int k = 0; k < 1000; k++) {
      P[i][k] = 0.001;
    }
  }

  threshold = 0;
  for (int i = 0; i < 1000; i++) {
    for (int j = 0; j < 1000; j++) {
      double p = P[i][j];
      double weight = f(p);
      threshold += weight * p;
      threshold += weight / 1000;
    }
  }
  threshold /= 2;
}

const char* classify(const vector<int>& v) {
  CHECK(v.size() == 1000);
  double sum = 0;
  for (size_t j = 0; j < v.size(); j++) {
    sum += f(P[j][v[j]]);
  }
  return sum <= threshold ? "GOOD" : "BAD";
}

int main() {
  initialize();
  run(&classify);
}
