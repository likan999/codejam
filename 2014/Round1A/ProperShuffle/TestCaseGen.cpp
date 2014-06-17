#include <cstdlib>
#include <ctime>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>
using namespace std;

void generateGood() {
  vector<int> data(1000);
  iota(data.begin(), data.end(), 0);
  for (int i = 0; i < 1000; i++) {
    int j = rand() % (1000 - i) + i;
    swap(data[i], data[j]);
  }
  for (int i = 0; i < 1000; i++) {
    if (i > 0) cout << " ";
    cout << data[i];
  }
  cout << endl;
}

void generateBad() {
  vector<int> data(1000);
  iota(data.begin(), data.end(), 0);
  for (int i = 0; i < 1000; i++) {
    int j = rand() % 1000;
    swap(data[i], data[j]);
  }
  for (int i = 0; i < 1000; i++) {
    if (i > 0) cout << " ";
    cout << data[i];
  }
  cout << endl;
}

int main() {
  srand(time(nullptr));
  cout << 120 << endl;
  for (int i = 0; i < 120; i++) {
    bool good = rand() % 2;
    cerr << (good ? "GOOD" : "BAD") << endl;
    cout << 1000 << endl;
    if (good) {
      generateGood();
    } else {
      generateBad();
    }
  }
}
