#pragma once
#include <random>
#include <chrono>

#include "Triangle.h"

class RandomGenerator {
private:

  mutable std::default_random_engine generator_;
  mutable std::uniform_int_distribution<int> distributionY_;
  mutable std::uniform_int_distribution<int> distributionX_;


  int RandomX() const {
    return distributionX_(generator_);
  }

  int RandomY() const {
    return distributionY_(generator_);
  }
public:
  RandomGenerator(const int width, const int height){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    generator_ = std::default_random_engine(seed);

    distributionX_ = std::uniform_int_distribution<int>(0, width - 1);
    distributionY_ = std::uniform_int_distribution<int>(0, height - 1);
  }

  Point RandomPoint() const {
    return Point(RandomX(), RandomY());
  }
};