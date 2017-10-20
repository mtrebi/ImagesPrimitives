#pragma once
#include <random>
#include <chrono>
#include <array>
#include "Vector.h"
#include "Point.h"
#include "Utils.h"
#include "Shape.h"

#include "../lib/EasyBMP_1.06/EasyBMP.h"

extern int WIDTH;
extern int HEIGHT;



class Triangle : public Shape {
private:
  Point v0_,
    v1_,
    v2_,
    pv0_,
    pv1_,
    pv2_;

  // TODO Mutator!
  mutable std::default_random_engine generator_;

  mutable std::uniform_int_distribution<int> mutation_vertex_distribution_;
  mutable std::uniform_int_distribution<int> mutation_x_distribution_;
  mutable std::uniform_int_distribution<int> mutation_y_distribution_;

  void BarycentricCoords(double& u, double& v, double& w, const Point& point) const {
    const Vector v0 = this->v1_ - this->v0_, v1 = this->v2_ - this->v0_, v2 = point - this->v0_;
    double d00 = v0 * v0;
    double d01 = v0 * v1;
    double d11 = v1 * v1;
    double d20 = v2 * v0;
    double d21 = v2 * v1;
    double denom = d00 * d11 - d01 * d01;
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;
  }

  float Area() const {
    return abs(((v1_.x - v0_.x) * (v2_.y - v0_.y)) - (v1_.y - v0_.y) * (v2_.x - v0_.x));
  }

  Point VertexMutation(const Point& vertex) const {
    int rx = mutation_x_distribution_(generator_),
      ry = mutation_y_distribution_(generator_);
    
    const Point mutated = Point(
      Utils::Clamp(vertex.x + rx, 0, WIDTH - 1),
      Utils::Clamp(vertex.y + ry, 0, HEIGHT - 1)
    );

    return mutated;
  }

public:
  Triangle() : 
    v0_(Point()), v1_(Point()), v2_(Point()) { 
  
  }

  Triangle(const Point& v1, const Point& v2, const Point& v3)
    : v0_(v1), v1_(v2), v2_(v3),
    pv0_(v1), pv1_(v2), pv2_(v3)
  
  {

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    generator_ = std::default_random_engine(seed);

    mutation_vertex_distribution_ = std::uniform_int_distribution<int>(0, 2);
    mutation_x_distribution_ = std::uniform_int_distribution<int>(-WIDTH / 2, WIDTH / 2);
    mutation_y_distribution_ = std::uniform_int_distribution<int>(-HEIGHT / 2, HEIGHT / 2);
  }


  virtual bool Contains(const Point& point) const override{
    double u, v, w;
    BarycentricCoords(u, v, w, point);
    return (u >= 0 && v >= 0 && u + v <= 1.0);
  }

  virtual bool Valid() const override {
    return Area() != 0;
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

      const int random_vertex = mutation_vertex_distribution_(generator_);
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
  }

  void Rollback() {
    v0_ = pv0_;
    v1_ = pv1_;
    v2_ = pv2_;
  }
};