#include <algorithm>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

#include "Common/Executor.h"
#include "Common/Io.h"

constexpr char Format[] = "%d:%d %d:%d";

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

template <typename Trip>
pair<vector<int>, vector<int>> computeTraintable(const vector<Trip>& trips, int turnaround) {
  pair<vector<int>, vector<int>> table;
  for (const auto& t: trips) {
    table.first.emplace_back(get<1>(t) * 60 + get<2>(t));
    table.second.emplace_back(get<3>(t) * 60 + get<4>(t) + turnaround);
  }
  sort(table.first.begin(), table.first.end());
  sort(table.second.begin(), table.second.end());

  return table;
}

pair<int, int> compute(
    int turnaround,
    io::PackedVector<2>,
    const vector<tuple<io::Scanf<4, Format>, int, int, int, int>>& aTrips,
    const vector<tuple<io::Scanf<4, Format>, int, int, int, int>>& bTrips) {
  vector<int> aDeparture, aArrival;
  vector<int> bDeparture, bArrival;
  tie(aDeparture, aArrival) = computeTraintable(aTrips, turnaround);
  tie(bDeparture, bArrival) = computeTraintable(bTrips, turnaround);

  return make_pair(computeOneSide(aDeparture, bArrival), computeOneSide(bDeparture, aArrival));
}

std::unique_ptr<Executor> Executor::instance(new FunctionalExecutor(&compute));
