#pragma once

#include "Image.h"

class ShapeMutator {
private:
  Image target_;
public:
  const int MUTATIONS = 50;

  ShapeMutator(const Image& image)
    : target_(image) {

  }

  void Mutate(std::shared_ptr<Shape> original) {
    original->Mutate();
    original->SetColor(target_);
  }

  void MutateBest(float& best_energy, const Image& current, std::shared_ptr<Shape> shape) {
    best_energy = std::numeric_limits<float>::max();

    for (int i = 0; i < MUTATIONS; ++i) {
      Mutate(shape);

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
