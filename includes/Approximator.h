#pragma once
#include "Image.h"
#include "Triangle.h"
#include "ShapeGenerator.h"
#include "ShapeMutator.h"
#include "Utils.h"
#include "ThreadPool.h"
#include <memory>

class Approximator {
private:
  Image current_,
        target_;

  float current_energy_ = 0;

  ShapeGenerator shape_generator_;
  ShapeMutator shape_mutator_;

  ThreadPool thread_pool_;

  void Init() {
    current_.SetBaseColor(target_.AverageColor());
    current_energy_ = Utils::Energy(target_, current_);
    thread_pool_.init();
  }

  
  /*
  std::shared_ptr<Shape> HillClimber(const Image& current) {
    float energy;
    std::shared_ptr<Shape> new_shape = shape_generator_.GenerateBest(current, SHAPE_TYPE);
    shape_mutator_.MutateBest(energy, current, new_shape);
  }*/

  /*
  Image Move(float &best_energy, const Image& current) {
    std::shared_ptr<Shape> best_shape;
    std::future<std::shared_ptr<Shape>> results[N_HC];
    best_energy = std::numeric_limits<float>::max();

    for (int i = 0; i < N_HC; ++i) {
      //float energy_ref;
      //auto f = thread_pool_.submit(HillClimber, std::ref(energy_ref));
      //results[i].future = f;
      //results[i].energy = energy_ref;
    }

    for (int i = 0; i < N_HC; ++i) {
      if (results[i].valid()) {
        std::shared_ptr<Shape> shape = results[i].get();
        // Get Shape Energy
        if (shape_energy < best_energy) {
          best_shape = shape;
          best_energy = shape_energy;
        }

      }

    }

    Image next = current;
    best_energy = std::numeric_limits<float>::max();
    std::shared_ptr<Shape> best_shape;
    for (int i = 0; i < N_HC; ++i) {
      float energy;
      std::shared_ptr<Shape> new_shape = shape_generator_.GenerateBest(current, SHAPE_TYPE);
      shape_mutator_.MutateBest(energy, current, new_shape);
      if (energy < best_energy) {
        best_shape = new_shape;
        best_energy = energy;
      }
    }
    next.AddShape(best_shape);
    return next;
  }
  */
  
  std::shared_ptr<Shape> Move(const Image& current) {
    std::shared_ptr<Shape> best_shape;
    for (int i = 0; i < N_HC; ++i) {
      std::shared_ptr<Shape> new_shape = shape_generator_.GenerateBest(current, SHAPE_TYPE);
      shape_mutator_.MutateBest(current, new_shape);
      if (!best_shape || new_shape->GetEnergy() < best_shape->GetEnergy()) {
        best_shape = new_shape;
      }
    }
    return best_shape;
  }
  // TODO Optimize score differencePartial
  // TODO Aliasing
public:
  static const ShapeType SHAPE_TYPE = ShapeType::TRIANGLE;
  static const int MAX_ITERATIONS = 50;
  const float MIN_ENERGY = 0.025f;
  static const int N_HC = 15;
  static const int ALPHA = 128;
  static const int N_THREADS = 1;

  Approximator(const Image& target) :
    current_(target.GetWidth(), target.GetHeight(), target.GetBitDepth(), target.GetHDPI(), target.GetVDPI()),
    target_(target),
    shape_generator_(target_, ALPHA),
    shape_mutator_(target_, ALPHA),
    thread_pool_(7) {
  }

  Image Run() {
    Init();
    
    auto generator = std::default_random_engine();
    auto distribution = std::uniform_int_distribution<int>(0, 1);
    
    int iterations = MAX_ITERATIONS;
    while (current_energy_ > MIN_ENERGY && iterations > 0) {
      std::cout << "Iteration " << MAX_ITERATIONS - iterations << " - Score " << current_energy_ << std::endl;

      std::shared_ptr<Shape> new_shape = Move(current_); 

      if (!new_shape) {
        continue;
      }

      const float delta_energy = new_shape->GetEnergy() - current_energy_;
      if (delta_energy < 0) {
        current_energy_ = new_shape->GetEnergy();
        current_.AddShape(new_shape);
        --iterations;
      }
      else if (exp(-delta_energy / iterations) > distribution(generator)){
        current_energy_ = new_shape->GetEnergy();
        current_.AddShape(new_shape);
        --iterations;
      }
    }
    return current_;
  }
};