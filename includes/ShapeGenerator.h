#pragma once
#include "RandomGenerator.h"
#include "Image.h"
#include "Ellipse.h"
#include <memory>

enum class ShapeType {
  TRIANGLE,
  ELLIPSE
};

class ShapeGenerator {
private:
  Image target_;
  RandomGenerator big_generator_x_, // From 0 to width
                  big_generator_y_, // From 0 to heigth
                  small_generator_;


  int alpha_;
  std::shared_ptr<Shape> GenerateTriangle() const {
    std::shared_ptr<Shape> shape = nullptr;

    while (!shape || !shape->Valid()) {
      Point v0(big_generator_x_.Random(), big_generator_y_.Random());
      Point v1(v0.x + small_generator_.Random(), v0.y + small_generator_.Random());
      Point v2(v0.x + small_generator_.Random(), v0.y + small_generator_.Random());

      shape = std::shared_ptr<Shape>(new Triangle(v0, v1, v2, target_.GetWidth(), target_.GetHeight()));
    }
    return shape;
  }

  std::shared_ptr<Shape> GenerateEllipse() const {
    return nullptr;// TODO
    //return std::shared_ptr<Shape>(new Ellipse(generator_.RandomPoint(), generator_.RandomX(), generator_.RandomY(), target_.GetWidth(), target_.GetHeight()));
  }

  std::shared_ptr<Shape> ShapeConstructor(const ShapeType type) const {
    std::shared_ptr<Shape> shape;
    switch (type) {
    case ShapeType::TRIANGLE:
      shape = GenerateTriangle();
      break;
    case ShapeType::ELLIPSE:
      shape = GenerateEllipse();
      break;
    default:
      shape =  nullptr;
    }
    return shape;
  }

  std::shared_ptr<Shape> Generate(const ShapeType type, const Image& current) const {
    std::shared_ptr<Shape> shape = ShapeConstructor(type);
    while (!shape->Valid()) {
      shape = ShapeConstructor(type);
    }
    shape->ComputeColor(target_, current, alpha_);
    return shape;
  }

public:
  const int RANDOM_BEST = 100;


  ShapeGenerator(const Image& image, const int alpha)
    : target_(image), 
    big_generator_x_(0, target_.GetWidth() - 1, 3),
    big_generator_y_(0, target_.GetHeight() - 1, 7),
    small_generator_(-std::min(target_.GetWidth(), target_.GetHeight()) / 32, std::min(target_.GetWidth(), target_.GetHeight()) / 32, 1),
    alpha_(alpha) {


  }

  std::shared_ptr<Shape> GenerateBest(const Image& current, const ShapeType type) const {
    float best_energy = std::numeric_limits<float>::max();
    std::shared_ptr<Shape> best_shape = nullptr;
    for (int i = 0; i < RANDOM_BEST; ++i) {
      std::shared_ptr<Shape> new_shape = Generate(type, current);
      const float new_energy = Utils::Energy(target_, current, new_shape);

      if (new_energy < best_energy) {
        best_shape = new_shape;
        best_energy = new_energy;
      }
    }
    return best_shape;
  }

};