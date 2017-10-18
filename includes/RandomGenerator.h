#pragma once
#include <random>
#include <chrono>

#include "Triangle.h"

class RandomGenerator {
private:
  std::default_random_engine generatorX_;
  std::uniform_int_distribution<int> distributionX_;

  std::default_random_engine generatorY_;
  std::uniform_int_distribution<int> distributionY_;

  std::default_random_engine generatorC_;
  std::uniform_int_distribution<int> distributionC_;

  int RandomX() const {
    return distributionX_(generatorX_);
  }

  int RandomY() const {
    return distributionY_(generatorY_);
  }
public:
  RandomGenerator(const int width, const int height){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    std::default_random_engine generatorX_(seed);
    std::default_random_engine generatorY_(seed);
    std::default_random_engine generatorC_(seed);


    distributionX_ = std::uniform_int_distribution<int>(0, width - 1);
    distributionY_ = std::uniform_int_distribution<int>(0, height - 1);
    distributionC_ = std::uniform_int_distribution<int>(0, 255);
  }

  Point RandomPoint() const {
    return Point(RandomX(), RandomY());
  }

  RGBApixel RandomColor() const {
    RGBApixel color;
    color.Red = distributionY_(generatorY_);
    color.Green = distributionY_(generatorY_);
    color.Blue = distributionY_(generatorY_);
    return color;
  }
};