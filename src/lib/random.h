#pragma once
#include <chrono>
#include <random>

inline int random(int start, int end) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 eng(seed);

  std::uniform_int_distribution<int> dist(start, end);

  return dist(eng);
}
