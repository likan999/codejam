#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

#define CHECK(expr) do { if (!(expr)) { cerr << __FILE__ << "(" << __LINE__ << ": " << #expr << " failed"; exit(1); } } while(false)

double f(double p) {
  return 1000 * p;
}

int main() {
  vector<vector<double>> P(1000);
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

  double threshold = 0;
  for (int i = 0; i < 1000; i++) {
    for (int j = 0; j < 1000; j++) {
      double p = P[i][j];
      double weight = f(p);
      threshold += weight * p;
      threshold += weight / 1000;
    }
  }
  threshold /= 2;

  int numCases;
  cin >> numCases;
  CHECK(numCases == 120);
  for (int i = 0; i < numCases; i++) {
    int n;
    cin >> n;
    CHECK(n == 1000);
    double sum = 0;
    for (int j = 0; j < n; j++) {
      int value;
      cin >> value;
      sum += f(P[j][value]);
    }
    cout << "Case #" << (i + 1) << ": " << (sum <= threshold ? "GOOD" : "BAD") << endl;
  }
}
