#pragma once
#include <limits>
#include "Point.h"
class Image;
#include "../lib/EasyBMP_1.06/EasyBMP.h"

struct BoundingBox {
  Point min, max;

  BoundingBox()
    : min(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()),
    max(std::numeric_limits<int>::min(), std::numeric_limits<int>::min()) {
  }
};

class Shape {
protected:
  int max_height_, max_width_;
private:
  RGBApixel color_;
  float energy_ = std::numeric_limits<float>::max(),
    penergy_ = std::numeric_limits<float>::max();

public:
  Shape(const int width, const int height)
    : max_height_(height), max_width_(width) {

  }

  float GetEnergy() const { return energy_; }
  void SetEnergy(const float energy) { 
    penergy_ = energy_;
    energy_ = energy; 
  }

  RGBApixel GetColor() const { return color_; }
  void SetColor(const RGBApixel& color) { color_ = color; }

  virtual bool Contains(const Point& point) const = 0;
  virtual bool Valid() const = 0;
  virtual BoundingBox GetBBox() const = 0;
  virtual void Mutate() = 0;
  virtual void Rollback() {
    energy_ = penergy_;
  }

  void ComputeColor(const Image& target, const Image& current, const int alpha); 
};
