#pragma once
#pragma once
#include <random>
#include <chrono>
#include <array>
#include "Vector.h"
#include "Point.h"
#include "Utils.h"
#include "Shape.h"

#include "../lib/EasyBMP_1.06/EasyBMP.h"

class Ellipse : public Shape {
private:
  Point c_;
  int rx_, ry_;

public:
  Ellipse(RandomGenerator& generator, const int width, const int height)
    : Shape(generator, width, height) {
    c_.x = generator_->Random(0, width_);
    c_.y = generator_->Random(0, width_);

    rx_ = generator_->Random(1, 31);
    ry_ = generator_->Random(1, 31);
  }

  virtual ~Ellipse() { }


  std::shared_ptr<Shape> Copy() const {
    std::shared_ptr<Shape> copy(new Ellipse(*this));
    return copy;
  }

  std::vector<Scanline>& Rasterize() {
    if (lines_.size() == 0 || has_changed_) {
      lines_.clear();

      const float aspect = (float) rx_ / ry_;
      for (int dy = 0; dy <= ry_; ++dy) {
        const int y1 = c_.y - dy;
        const int y2 = c_.y + dy;

        if ((y1 < 0 || y1 >= height_) && (y2 < 0 || y2 >= height_)) {
          continue;
        }

        const int s = sqrt(ry_ * ry_ - dy * dy) * aspect;
        int x1 = c_.x - s;
        int x2 = c_.x + s;

        x1 = Utils::Clamp(x1, 0, width_ - 1);
        x2 = Utils::Clamp(x2, 0, width_ - 1);

        if (y1 >= 0 && y1 < height_) {
          lines_.push_back(Scanline(y1, x1, x2));
        }

        if (y2 >= 0 && y2 < height_ && dy > 0) {
          lines_.push_back(Scanline(y2, x1, x2));
        }

      }
      has_changed_ = false;
    }
    return lines_;
  }

  void Mutate() {
    has_changed_ = true;
    const int random_vertex = generator_->Random(0, 2);
    switch (random_vertex) {
    case 0:
      c_.x = Utils::Clamp(c_.x + generator_->Random(-16, 16), 0, width_ - 1);
      c_.y = Utils::Clamp(c_.y + generator_->Random(-16, 16), 0, height_ - 1);
      break;
    case 1:
      rx_ = Utils::Clamp(rx_ + generator_->Random(-16, 16), 1, width_ - 1);
      break;
    case 2:
      ry_ = Utils::Clamp(ry_ + generator_->Random(-16, 16), 1, height_ - 1);
      break;
    }
  }
};