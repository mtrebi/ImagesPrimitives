#pragma once
#include "Point.h"
#include "Scanline.h"
#include <vector>
#include <limits>
#include "RandomGenerator.h"

class Shape {
protected:
  int width_, height_, max_random_;
  std::vector<Scanline> lines_;
  bool has_changed_ = true;
  RandomGenerator* generator_;
public:
  Shape(RandomGenerator& g, const int w, const int h, const int mr) : generator_(&g), width_(w), height_(h), max_random_(mr){}

  virtual ~Shape(){ }

  virtual std::vector<Scanline>& Rasterize() = 0;
  virtual void Mutate() = 0;
  virtual std::shared_ptr<Shape> Copy() const = 0;
};
