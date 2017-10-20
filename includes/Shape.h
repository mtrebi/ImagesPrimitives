#pragma once
#include <limits>
#include "Point.h"

#include "../lib/EasyBMP_1.06/EasyBMP.h"

struct BoundingBox {
  Point min, max;

  BoundingBox()
    : min(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()),
    max(std::numeric_limits<int>::min(), std::numeric_limits<int>::min()) {
  }
};

class Shape {
private:
  RGBApixel color_;

public:

  RGBApixel GetColor() const { return color_; }
  void SetColor(const RGBApixel& color) { color_ = color; }

  virtual bool Contains(const Point& point) const = 0;
  virtual bool Valid() const = 0;
  virtual BoundingBox GetBBox() const = 0;
  virtual Shape * Mutate() const = 0;

  void AddToImage(Image& image) const {
    const BoundingBox bbox = this->GetBBox();
    for (int x = bbox.min.x; x < bbox.max.x; ++x) {
      for (int y = bbox.min.y; y < bbox.max.y; ++y) {
        if (this->Contains(Point(x, y))) {
          image.SetPixel(x, y, this->GetColor());
        }
      }
    }
  }
};
