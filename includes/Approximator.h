#pragma once
#include "Image.h"
#include "Triangle.h"
#include "ShapeGenerator.h"
#include "ShapeMutator.h"
#include "Utils.h"
#include <memory>

class Approximator {
private:
  Image current_,
        target_;

  float current_energy_ = 0;

  ShapeGenerator shape_generator_;
  ShapeMutator shape_mutator_;

  void Init() {
    current_.SetBaseColor(target_.AverageColor());
    current_energy_ = Utils::Energy(target_, current_);
  }

  Image Move(float &best_energy, const Image& current) {
    Image next = current;
    best_energy = std::numeric_limits<float>::max();
    std::shared_ptr<Shape> best_shape;
    for (int i = 0; i < N_HC; ++i) { // TODO While no improvement_
      float energy;
      //const Shape * new_shape = shape_mutator_.Mutate(energy, shape_generator_.Generate(SHAPE_TYPE));
      std::shared_ptr<Shape> new_shape = shape_generator_.GenerateBest(current, SHAPE_TYPE);
      shape_mutator_.MutateBest(energy, current, new_shape);
      if (energy < best_energy) {
        best_shape = new_shape;
        best_energy = energy;
      }
    }

    best_shape->AddToImage(next);
    return next;
  }
  // TODO Optimizacion copiar solo bounding box de pixeles
  // TODO Optimizacion bounding box score parcial
  // TODO Threading by working only on a sub-part of the image
public:
  const ShapeType SHAPE_TYPE = ShapeType::ELLIPSE;
  const int MAX_ITERATIONS = 2000;
  const int MIN_ENERGY = 15;
  const int N_HC = 15;

  Approximator(const Image& target) :
    current_(target.GetWidth(), target.GetHeight(), target.GetBitDepth(), target.GetHDPI(), target.GetVDPI()),
    target_(target),
    shape_generator_(target_),
    shape_mutator_(target_)
    {
  }

  Image Run() {
    Init();
    
    auto generator = std::default_random_engine();
    auto distribution = std::uniform_int_distribution<int>(0, 1);
    
    int iterations = 0;
    while (current_energy_ > MIN_ENERGY && iterations < MAX_ITERATIONS) {
      std::cout << "Iteration " << iterations << " - Score " << current_energy_ << std::endl;
      float next_energy;
      Image next = Move(next_energy, current_);
      float delta_energy = next_energy - current_energy_;

      if (delta_energy < 0) {
        current_energy_ = next_energy;
        current_ = next;
      }
      else if (exp(-delta_energy / iterations) > distribution(generator)){
        current_energy_ = next_energy;
        current_ = next;
      }
      
      ++iterations;
    }
    return current_;
  }
};