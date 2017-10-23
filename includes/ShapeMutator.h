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
  const int MUTATIONS = 100;

  ShapeMutator(const Image& image, const int alpha)
    : target_(image), alpha_(alpha) {

  }

  void MutateBest(float& best_energy, const Image& current, std::shared_ptr<Shape> shape) {
    best_energy = std::numeric_limits<float>::max();

    for (int i = 0; i < MUTATIONS; ++i) {
      Mutate(shape, current);

      float new_energy = Utils::Energy(target_, current, shape);
      if (new_energy < best_energy) {
        best_energy = new_energy;
      }
      else {
        shape->Rollback();
      }
    };
  }
};
