#pragma once
#include <random>

class RandomGenerator {
private:

  mutable std::default_random_engine generator_;
  mutable std::uniform_int_distribution<int> distribution_;

public:
  RandomGenerator(){}
  
  RandomGenerator(const int min, const int max, const unsigned seed){
    unsigned real_seed = std::chrono::system_clock::now().time_since_epoch().count();

    generator_ = std::default_random_engine(real_seed + seed);
    distribution_ = std::uniform_int_distribution<int>(min, max);
  }

  int Random() const {
    return distribution_(generator_);
  }
};