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

  Shape * Mutate(const Shape * original) {
    Shape * mutation = original->Mutate();
    mutation->SetColor(target_);
    return mutation;
  }

  const Shape * MutateBest(float& best_energy, const Image& current, const Shape * original) {
    best_energy = std::numeric_limits<float>::max();
    const Shape * best_shape = original;

    for (int i = 0; i < MUTATIONS; ++i) {
      Shape * new_mutation = Mutate(best_shape);

      float new_energy = Utils::Energy(target_, current, new_mutation);
      if (new_energy < best_energy) {
        if (best_shape && best_shape != original) delete best_shape;
        best_shape = new_mutation;
        best_energy = new_energy;
      }
    }

    return best_shape;
  }
};
