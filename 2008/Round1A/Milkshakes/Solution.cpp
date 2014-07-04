#include <string>
#include <unordered_set>
#include <vector>

#include <boost/variant/variant.hpp>

#include "Common/Executor.h"

using namespace std;

struct Customer {
  int malted = -1;
  unordered_set<int> unmalted;
  bool satisfied = false;
};

boost::variant<string, vector<int>> compute(int n, const vector<vector<pair<int, bool>>>& c) {
  int m = c.size();
  vector<Customer> customers(m);
  vector<vector<Customer*>> maltedCustomers(n);
  vector<vector<Customer*>> unMaltedCustomers(n);
  vector<Customer*> next;
  vector<int> choices(n);
  for (int i = 0; i < m; i++) {
    Customer& customer = customers[i];
    for (const auto& p: c[i]) {
      int flavor = p.first - 1;
      bool malted = p.second;
      if (malted) {
        customer.malted = flavor;
        maltedCustomers[flavor].emplace_back(&customer);
      } else {
        customer.unmalted.insert(flavor);
        unMaltedCustomers[flavor].emplace_back(&customer);
      }
    }
    if (customer.unmalted.empty()) {
      next.emplace_back(&customer);
    }
  }
  while (!next.empty()) {
    Customer* customer = next.back();
    next.pop_back();
    if (!customer->satisfied) {
      int flavor = customer->malted;
      if (flavor == -1) {
        return "IMPOSSIBLE";
      }
      choices[flavor] = 1;
      for (auto* cc: maltedCustomers[flavor]) {
        cc->satisfied = true;
      }
      for (auto* cc: unMaltedCustomers[flavor]) {
        if (!cc->satisfied && cc->unmalted.find(flavor) != cc->unmalted.end()) {
          cc->unmalted.erase(flavor);
          if (cc->unmalted.empty()) {
            next.emplace_back(cc);
          }
        }
      }
    }
  }
  return choices;
}

std::unique_ptr<Executor> Executor::instance(new FunctionalExecutor(&compute));
