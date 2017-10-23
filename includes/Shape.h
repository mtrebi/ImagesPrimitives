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
public:

  void SetSize(const int width, const int height) {
    max_height_ = height;
    max_width_ = width;
  }

  RGBApixel GetColor() const { return color_; }
  void SetColor(const RGBApixel& color) { color_ = color; }

  virtual bool Contains(const Point& point) const = 0;
  virtual bool Valid() const = 0;
  virtual BoundingBox GetBBox() const = 0;
  virtual void Mutate() = 0;
  virtual void Rollback() = 0;

  void SetColor(const Image& target, const Image& current, const int alpha); 
};
