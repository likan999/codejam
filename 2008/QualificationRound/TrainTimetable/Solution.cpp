#include <algorithm>
#include <iostream>
#include <sstream>
#include <utility>

using namespace std;

#include "Common/Executor.h"

pair<vector<int>, vector<int>> readTrips(int n, int turnaround) {
  pair<vector<int>, vector<int>> trips;
  for (int i = 0; i < n; i++) {
    string departure, arrival;
    cin >> departure >> arrival;
    departure[2] = ' ';
    arrival[2] = ' ';
    stringstream departureSS(departure);
    int hour, minute;
    departureSS >> hour >> minute;
    trips.first.emplace_back(hour * 60 + minute);
    stringstream arrivalSS(arrival);
    arrivalSS >> hour >> minute;
    trips.second.emplace_back(hour * 60 + minute + turnaround);
  }
  sort(trips.first.begin(), trips.first.end());
  sort(trips.second.begin(), trips.second.end());
  return trips;
}

int computeOneSide(const vector<int>& departure, const vector<int>& arrival) {
  int total = 0;
  int nextArrival = 0;
  for (int departureTime: departure) {
    if (nextArrival < arrival.size() && departureTime >= arrival[nextArrival]) {
      ++nextArrival;
    } else {
      ++total;
    }
  }
  return total;
}

pair<int, int> compute() {
  int turnaround;
  cin >> turnaround;
  int nA, nB;
  cin >> nA >> nB;
  auto aTrips = readTrips(nA, turnaround);
  auto bTrips = readTrips(nB, turnaround);

  return make_pair(computeOneSide(aTrips.first, bTrips.second), computeOneSide(bTrips.first, aTrips.second));
}

std::unique_ptr<Executor> Executor::instance(new FunctionalExecutor(&compute));
