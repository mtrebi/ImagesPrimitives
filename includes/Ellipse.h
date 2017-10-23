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
  Point c_, pc_;
  int rx_, prx_, ry_, pry_;

  mutable std::default_random_engine generator_;

  mutable std::uniform_int_distribution<int> mutation_attr_distribution_;
  mutable std::uniform_int_distribution<int> mutation_x_distribution_;
  mutable std::uniform_int_distribution<int> mutation_y_distribution_;

  Point CenterMutation() const {
    int rx = mutation_x_distribution_(generator_),
        ry = mutation_y_distribution_(generator_);

    const Point mutated = Point(
      Utils::Clamp(c_.x + rx, 0, max_width_ - 1),
      Utils::Clamp(c_.y + ry, 0, max_height_ - 1)
    );

    return mutated;
  }

  int RadiusXMutation() const {
    const int mutation = mutation_x_distribution_(generator_);
    const int mutation_clamped = Utils::Clamp(mutation + rx_, 0, max_width_ - 1);
    return mutation_clamped;
  }

  int RadiusYMutation() const {
    const int mutation = mutation_y_distribution_(generator_);
    const int mutation_clamped = Utils::Clamp(mutation + rx_, 0, max_height_ - 1);
    return mutation_clamped;
  }

public:
  Ellipse() :
    Shape(0,0), c_(Point()), rx_(0), ry_(0) {

  }

  Ellipse(const Point& c, const int rx, const int ry, const int width, const int height)
    : Shape(width, height),
    c_(c), rx_(rx), ry_(ry),
     pc_(c), prx_(rx), pry_(ry)
  {

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    generator_ = std::default_random_engine(seed);

    mutation_attr_distribution_ = std::uniform_int_distribution<int>(0, 2);
    mutation_x_distribution_ = std::uniform_int_distribution<int>(-max_width_ / 2, max_width_ / 2);
    mutation_y_distribution_ = std::uniform_int_distribution<int>(-max_height_ / 2, max_height_ / 2);
  }


  virtual bool Contains(const Point& point) const override {
    return (pow(point.x - c_.x, 2) / (rx_ * rx_)) + (pow(point.y - c_.y, 2) / (ry_ * ry_)) <= 1;
  }

  virtual bool Valid() const override {
    BoundingBox bbox = GetBBox();
    return rx_ > 0 && ry_ > 0 &&
      (bbox.min.x >= 0 && bbox.min.x <= max_width_ - 1) &&
      (bbox.min.y >= 0 && bbox.min.y <= max_height_ - 1) &&
      (bbox.max.x >= 0 && bbox.max.x <= max_width_ - 1) &&
      (bbox.max.y >= 0 && bbox.max.y <= max_height_ - 1);
  }

  virtual BoundingBox GetBBox() const override {
    BoundingBox bbox;
    bbox.min = Point(c_.x - rx_, c_.y - ry_);
    bbox.max = Point(c_.x + rx_, c_.y + ry_);
    return bbox;
  }

  void Mutate() {
    while (true) {
      this->pc_ = this->c_;
      this->prx_ = this->rx_;
      this->pry_ = this->ry_;
      
      const int random_vertex = mutation_attr_distribution_(generator_);

      switch (random_vertex) {
      case 0:
        this->c_ = CenterMutation();
        break;
      case 1:
        this->rx_ = RadiusXMutation();
        break;
      case 2:
        this->ry_ = RadiusYMutation();
        break;
      default:
        throw std::invalid_argument("rand is out of boundaries");
      }

      if (!Valid()) {
        Rollback();
      }
      else {
        break;
      }
    }
  }

  void Rollback() override {
    this->c_ = this->pc_;
    this->rx_ = this->prx_;
    this->ry_ = this->pry_;
  }
};