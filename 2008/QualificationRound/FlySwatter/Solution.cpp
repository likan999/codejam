#include <cmath>
#include <memory>

using namespace std;

#include "Runner/Runner.h"

double square(double x) {
  return x * x;
}

double circle(double x) {
  return sqrt(1 - square(x));
}

double S(double x) {
  return 0.5 * (asin(x) + x * circle(x));
}

double computeSquare(double lx, double by, double g) {
  double rx = lx + g;
  double ty = by + g;
  double bxmax = circle(by);
  double lymax = circle(lx);
  if (bxmax <= rx) {
    if (lymax <= ty) {
      return S(bxmax) - S(lx) - (bxmax - lx) * by;
    } else {
      return S(ty) - S(by) - g * lx;
    }
  } else {
    if (lymax <= ty) {
      return S(rx) - S(lx) - g * by;
    } else {
      double tx = circle(ty);
      return (tx - lx) * g + S(rx) - S(tx) - (rx - tx) * by;
    }
  }
}

double compute(double f, double R, double t, double r, double g) {
  double inner = R - t - f;
  if (inner <= 0) {
    return 1;
  }
  g -= 2 * f;
  if (g <= 0) {
    return 1;
  }
  g /= inner;
  r = (r + f) / inner;
  double delta = g + 2 * r;

  double total = 0;

  for (double lx = r; lx < 1; lx += delta) {
    double rx = lx + g;
    double lymax = circle(lx);
    int n = 0;
    if (rx < 1) {
      double rymax = circle(rx);
      n = (int) floor((rymax + r) / delta);
      total += n * square(g);
    }
    for (double by = r + n * delta; by < lymax; by += delta) {
      total += computeSquare(lx, by, g);
    }
  }

  return 1 - total * square(inner / R) / atan(1);
}

std::unique_ptr<Runner> Runner::instance(new FunctionRunner(&compute));
