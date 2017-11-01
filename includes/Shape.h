#pragma once
#include "Point.h"
#include "Scanline.h"
#include <vector>
#include <limits>
#include "RandomGenerator.h"

class Shape {
protected:
  int width_, height_;
  std::vector<Scanline> lines_;
  bool has_changed_ = true;
  RandomGenerator* generator_;
public:
  Shape(RandomGenerator& g, const int w, const int h) : generator_(&g), width_(w), height_(h){}

  virtual ~Shape(){ }

  virtual std::vector<Scanline>& Rasterize() = 0;
  virtual void Mutate() = 0;
  virtual std::shared_ptr<Shape> Copy() const = 0;
};
