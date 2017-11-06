#pragma once
#include <random>
#include <chrono>
#include <array>
#include "Vector.h"
#include "Point.h"
#include "Utils.h"
#include "Shape.h"
#include "RandomGenerator.h"

#include "../lib/EasyBMP_1.06/EasyBMP.h"

class Triangle : public Shape {
private:
  Point v0_, v1_, v2_;

  float Area() const {
    return abs(((v1_.x - v0_.x) * (v2_.y - v0_.y)) - (v1_.y - v0_.y) * (v2_.x - v0_.x));
  }

  void RasterizeTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    if (y1 > y3) {
      Utils::Swap(x1, x3);
      Utils::Swap(y1, y3);
    }
    if (y1 > y2){
      Utils::Swap(x1, x2);
      Utils::Swap(y1, y2);
    }
    if (y2 > y3){
      Utils::Swap(x2, x3);
      Utils::Swap(y2, y3);
    }
    if (y2 == y3){
      RasterizeTriangleBottom(x1, y1, x2, y2, x3, y3);
    }
    else if (y1 == y2){
      RasterizeTriangleTop(x1, y1, x2, y2, x3, y3);
    }
    else {
      const int x4 = x1 + (((float)(y2 - y1)) / (y3 - y1)) * (x3 - x1);
      const int y4 = y2;
      RasterizeTriangleBottom(x1, y1, x2, y2, x4, y4);
      RasterizeTriangleTop(x2, y2, x4, y4, x3, y3);
    }
  }

  void RasterizeTriangleBottom(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3) {
    const float s1 = ((float)(x2 - x1)) / (y2 - y1);
    const float s2 = ((float)(x3 - x1)) / (y3 - y1);
    float ax = x1,
      bx = x1;

    for (int y = y1; y <= y2; ++y) {
      int a = ax;
      int b = bx;
      ax += s1;
      bx += s2;
      if (a > b) {
        Utils::Swap(a, b);
      }

      const int final_y = Utils::Clamp(y, 0, height_ - 1);
      const int final_a = Utils::Clamp(a, 0, width_ - 1);
      const int final_b = Utils::Clamp(b, 0, width_ - 1);

      lines_.push_back(Scanline(final_y, final_a, final_b));
    }
  }

  void RasterizeTriangleTop(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3) {
    std::vector<Scanline> lines;
    const float s1 = ((float)(x3 - x1)) / (y3 - y1);
    const float s2 = ((float)(x3 - x2)) / (y3 - y2);
    float ax = x3;
    float bx = x3;

    for (int y = y3; y > y1; --y) {
      ax -= s1;
      bx -= s2;
      int a = ax;
      int b = bx;
      if (a > b) {
        Utils::Swap(a, b);
      }
      const int final_y = Utils::Clamp(y, 0, height_ -1);
      const int final_a = Utils::Clamp(a, 0, width_ -1);
      const int final_b = Utils::Clamp(b, 0, width_ -1);

      lines_.push_back(Scanline(final_y, final_a, final_b));
    }
  }

public:  
  Triangle(RandomGenerator& generator, const int width, const int height, const int max_random)
    : Shape(generator, width, height, max_random){

    v0_.x = generator_->Random(0, width_);
    v0_.y = generator_->Random(0, height_);

    v1_.x = v0_.x + generator_->Random(-max_random_, max_random_);
    v1_.y = v0_.y + generator_->Random(-max_random_, max_random_);

    v2_.x = v0_.x + generator_->Random(-max_random_, max_random_);
    v2_.y = v0_.y + generator_->Random(-max_random_, max_random_);
  }

  virtual ~Triangle() { }

  std::shared_ptr<Shape> Copy() const {
    std::shared_ptr<Shape> copy(new Triangle(*this));
    return copy;
  }

  std::vector<Scanline>& Rasterize() {
    if (lines_.size() == 0 || has_changed_) {
      lines_.clear();
      RasterizeTriangle(v0_.x, v0_.y, v1_.x, v1_.y, v2_.x, v2_.y);
      has_changed_ = false;
    }
    return lines_;
  }

  bool Valid() const {
    return
      v0_.x >= 0 && v0_.x < width_ && v0_.y >= 0 && v0_.y < height_ &&
      v1_.x >= 0 && v1_.x < width_ && v1_.y >= 0 && v1_.y < height_ &&
      v2_.x >= 0 && v2_.x < width_ && v2_.y >= 0 && v2_.y < height_ &&
      Area() != 0;
  }

  void Mutate() {
    has_changed_ = true;
    while (true) {
      const int random_vertex = generator_->Random(0, 2);
      switch (random_vertex) {
      case 0:
        v0_.x = Utils::Clamp(v0_.x + generator_->Random(-max_random_, max_random_), 0, width_ - 1);
        v0_.y = Utils::Clamp(v0_.y + generator_->Random(-max_random_, max_random_), 0, height_ - 1);
        break;
      case 1:
        v1_.x = Utils::Clamp(v1_.x + generator_->Random(-max_random_, max_random_), 0, width_ - 1);
        v1_.y = Utils::Clamp(v1_.y + generator_->Random(-max_random_, max_random_), 0, height_ - 1);
        break;
      case 2:
        v2_.x = Utils::Clamp(v2_.x + generator_->Random(-max_random_, max_random_), 0, width_ - 1);
        v2_.y = Utils::Clamp(v2_.y + generator_->Random(-max_random_, max_random_), 0, height_ - 1);
        break;
      }
      if (Valid()) {
        break;
      }
    }
  }
};
