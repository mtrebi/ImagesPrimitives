#pragma once
#include "RandomGenerator.h"
#include "Image.h"
#include "Ellipse.h"

enum class ShapeType {
  TRIANGLE,
  ELLIPSE
};

class ShapeGenerator {
private:
  Image target_;
  RandomGenerator generator_;

  Triangle * GenerateTriangle() const {
    return new Triangle(generator_.RandomPoint(), generator_.RandomPoint(), generator_.RandomPoint());
  }

  Ellipse * GenerateEllipse() const {
    return new Ellipse(generator_.RandomPoint(), generator_.RandomX(), generator_.RandomY());
  }

  Shape * ShapeConstructor(const ShapeType type) const {
    switch (type) {
    case ShapeType::TRIANGLE:
      return GenerateTriangle();
    case ShapeType::ELLIPSE:
      return GenerateEllipse();
    default:
      return nullptr;
    }
  }
public:
  const int RANDOM_BEST = 15;

  ShapeGenerator(const Image& image)
    : target_(image), generator_(target_.GetWidth(), target_.GetHeight()) {

  }

  Shape * Generate(const ShapeType type) const {
    Shape * shape = ShapeConstructor(type);
    while (!shape->Valid()) {
      delete shape;
      shape = ShapeConstructor(type);
    }
    shape->SetColor(target_);
    return shape;
  }

  Shape * GenerateBest(const Image& current, const ShapeType type) const {
    float best_energy = std::numeric_limits<float>::max();
    Shape * best_shape = nullptr;
    for (int i = 0; i < RANDOM_BEST; ++i) {
      Shape * new_shape = Generate(type);
      const float new_energy = Utils::Energy(target_, current, new_shape);

      if (new_energy < best_energy) {
        if (best_shape) delete best_shape;
        best_shape = new_shape;
        best_energy = new_energy;
      }
    }
    return best_shape;
  }

};