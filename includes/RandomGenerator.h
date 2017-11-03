#pragma once
#pragma once
#include <random>
#include <chrono>
class RandomGenerator {
private:
  mutable std::uniform_int_distribution<int> uniform_dist{0, INT_MAX};
  mutable std::mt19937 generator_{ std::random_device{}() };

public:
  RandomGenerator(std::mt19937::result_type seed)
    : generator_(seed + std::chrono::high_resolution_clock::now().time_since_epoch().count()) {
  }

  RandomGenerator(const RandomGenerator& r) = delete;

  int Random(const int lower_bound, const int upper_bound) const {
    std::uniform_int_distribution<int> dist(lower_bound, upper_bound);
    return dist(generator_);
  }
};
