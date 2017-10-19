#pragma once
#include <random>
#include <chrono>
#include <array>
#include "Vector.h"
#include "Point.h"
#include "Utils.h"

#include "../lib/EasyBMP_1.06/EasyBMP.h"

extern int WIDTH;
extern int HEIGHT;

struct BoundingBox {
  Point min, max;

  BoundingBox()
    : min(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()),
      max(std::numeric_limits<int>::min(), std::numeric_limits<int>::min()) {
  }
};

class Triangle {
private:
  Point v0_, 
        v1_,
        v2_;

  RGBApixel color_;

  BoundingBox bbox_;

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

  void SetBBox() {
    std::array<Point, 3> vertices{ this->v0_, this->v1_, this->v2_ };

    for (auto& vertex : vertices) {
      if (vertex.x < bbox_.min.x) {
        bbox_.min.x = vertex.x;
      }
      if (vertex.y < bbox_.min.y) {
        bbox_.min.y = vertex.y;
      }

      if (vertex.x > bbox_.max.x) {
        bbox_.max.x = vertex.x;
      }
      if (vertex.y > bbox_.max.y) {
        bbox_.max.y = vertex.y;
      }
    }
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
    : v0_(v1), v1_(v2), v2_(v3) {

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    generator_ = std::default_random_engine(seed);

    mutation_vertex_distribution_ = std::uniform_int_distribution<int>(0, 2);
    mutation_x_distribution_ = std::uniform_int_distribution<int>(-WIDTH / 2, WIDTH / 2);
    mutation_y_distribution_ = std::uniform_int_distribution<int>(-HEIGHT / 2, HEIGHT / 2);

    SetBBox();
  }

  RGBApixel GetColor() const { return color_; }
  void SetColor(const RGBApixel& color) { color_ = color; }

  BoundingBox BBox() const {
    return bbox_;
  }

  bool Contains(const Point& point) const {
    double u, v, w;
    BarycentricCoords(u, v, w, point);
    return (u >= 0 && v >= 0 && u + v <= 1.0);
  }

  bool Valid() const {
    return Area() != 0;
  }

  float Area() const {
    return abs(((v1_.x - v0_.x) * (v2_.y - v0_.y)) - (v1_.y - v0_.y) * (v2_.x - v0_.x));
  }

  Triangle Mutate() const {
    //Triangle must be valid!
    Triangle triangle;
    while (!triangle.Valid()) {
      const int random_vertex = mutation_vertex_distribution_(generator_);
      switch (random_vertex) {
      case 0:
        triangle = Triangle(VertexMutation(v0_), v1_, v2_);
        break;
      case 1:
        triangle = Triangle(v0_, VertexMutation(v1_), v2_);
        break;
      case 2:
        triangle = Triangle(v0_, v1_, VertexMutation(v2_));
        break;
      default:
        throw std::invalid_argument("rand is out of boundaries");
      }
    }
    return triangle;
  }
};