#pragma once

#include "Image.h"

class ShapeMutator {
private:
  Image target_;
  int alpha_;
private:
  void Mutate(std::shared_ptr<Shape> original, const Image& current) {
    original->Mutate();
    original->SetColor(target_, current, alpha_);
  }

public:
  const int MUTATIONS = 200;

  ShapeMutator(const Image& image, const int alpha)
    : target_(image), alpha_(alpha) {

  }

  void MutateBest(const Image& current, std::shared_ptr<Shape> shape) {
    float best_energy = shape->GetEnergy();
    for (int i = 0; i < MUTATIONS; ++i) {
      Mutate(shape, current);

      Utils::Energy(target_, current, shape);
      if (shape->GetEnergy() < best_energy) {
        best_energy = shape->GetEnergy();
      }
      else {
        shape->Rollback();
      }
    };
  }
};
