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
  Point v0_,
    v1_,
    v2_,
    pv0_,
    pv1_,
    pv2_;

  // Barycentric cache
  Vector v0, v1;
  double d00, d01, d11, denom;


  RandomGenerator random_vertex_;
  RandomGenerator random_mutation_x_;
  RandomGenerator random_mutation_y_;

  
  void BarycentricCoords(double& u, double& v, double& w, const Point& point) const {
    const Vector v2 = point - this->v0_;
    double d20 = v2 * v0;
    double d21 = v2 * v1;
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;
  }

  float Area() const {
    return abs(((v1_.x - v0_.x) * (v2_.y - v0_.y)) - (v1_.y - v0_.y) * (v2_.x - v0_.x));
  }

  Point VertexMutation(const Point& vertex) const {
    const int rx = random_mutation_x_.Random();
    const int ry = random_mutation_y_.Random();
    
    const Point mutated = Point(
      Utils::Clamp(vertex.x + rx, 0, max_width_ - 1),
      Utils::Clamp(vertex.y + ry, 0, max_height_ - 1)
    );

    return mutated;
  }

  void UpdateBarycentricCache() {
    v0 = this->v1_ - this->v0_;
    v1 = this->v2_ - this->v0_;
    d00 = v0 * v0;
    d01 = v0 * v1;
    d11 = v1 * v1;
    denom = d00 * d11 - d01 * d01;
  }

public:
  Triangle() : 
    Shape(0,0), v0_(Point()), v1_(Point()), v2_(Point()) { 
  
  }

  Triangle(const Point& v1, const Point& v2, const Point& v3, const int width, const int height)
    : Shape(width, height),
    v0_(v1), v1_(v2), v2_(v3),
    pv0_(v1), pv1_(v2), pv2_(v3),
    random_vertex_(0, 2, 0),
    random_mutation_x_(-max_width_ / 32, max_width_/ 32, 11),
    random_mutation_y_(-max_height_ / 32, max_height_ / 32, 13) {

    UpdateBarycentricCache();
  }

  virtual bool Contains(const Point& point) const override{
    double u, v, w;
    BarycentricCoords(u, v, w, point);
    return (u >= 0 && v >= 0 && u + v <= 1.0);
  }

  virtual bool Valid() const override {
    return 
      v0_.x >= 0 && v0_.x < max_width_ && v0_.y >= 0 && v0_.y < max_height_ &&
      v1_.x >= 0 && v1_.x < max_width_ && v1_.y >= 0 && v1_.y < max_height_ &&
      v2_.x >= 0 && v2_.x < max_width_ && v2_.y >= 0 && v2_.y < max_height_ &&
      Area() != 0;
  }

  virtual BoundingBox GetBBox() const override {
    BoundingBox bbox;
    std::array<Point, 3> vertices{ this->v0_, this->v1_, this->v2_ };

    for (auto& vertex : vertices) {
      if (vertex.x < bbox.min.x) {
        bbox.min.x = vertex.x;
      }
      if (vertex.y < bbox.min.y) {
        bbox.min.y = vertex.y;
      }

      if (vertex.x > bbox.max.x) {
        bbox.max.x = vertex.x;
      }
      if (vertex.y > bbox.max.y) {
        bbox.max.y = vertex.y;
      }
    }
    return bbox;
  }

  void Mutate() {
    while (true) {
      this->pv0_ = this->v0_;
      this->pv1_ = this->v1_;
      this->pv2_ = this->v2_;

      const int random_vertex = random_vertex_.Random();
      switch (random_vertex) {
      case 0:
        v0_ = VertexMutation(v0_);
        break;
      case 1:
        v1_ = VertexMutation(v1_);
        break;
      case 2:
        v2_ = VertexMutation(v2_);
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
    UpdateBarycentricCache();
  }

  void Rollback() override {
    Shape::Rollback();
    v0_ = pv0_;
    v1_ = pv1_;
    v2_ = pv2_;
    UpdateBarycentricCache();
  }
};