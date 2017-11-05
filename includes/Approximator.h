#pragma once
#include "Model.h"
#include "State.h"


#include "Triangle.h"
#include "Utils.h"
#include "ThreadPool.h"
#include <memory>
#include <functional>
#include <string>

class Approximator {
private:
  Model model_;
  ctpl::thread_pool thread_pool_;
  
  void MultiThreadingBestHillClimb(Model& model, const ShapeType& shape_type, const int alpha, const int n_climbs, const int max_climbs, const int n_random_state) {
    std::future<State> results[kN_THREADS];
    for (int i = 0; i < kN_THREADS; ++i) {
      RandomGenerator generator(i);
      results[i] = thread_pool_.push(std::bind(&Model::BestHillClimb, std::ref(model_), std::ref(generator), shape_type, alpha, n_climbs, max_climbs, n_random_state));
    }

    State best_state, state;
    for (int i = 0; i < kN_THREADS; ++i) {
      if (results[i].valid()) {
        state = results[i].get();
        if (i == 0 || state.energy < best_state.energy) {
          best_state = state;
        }
      }
    }

    model_.Add(best_state.shape, best_state.alpha); 
  }

public:
  static const int kN_THREADS = 16;
  const int kN_SHAPES = 1500;
  const int kN_RANDOM_STATE = 1000;  
  const int kN_CLIMBS = 100;       
  const int kALPHA = 128;
  const int kMAX_CLIMBS = 1000;

  const std::string kPATH = "Output/lion.bmp";
  const std::string kPATH_GIF = "Output/gif/lion";
  const ShapeType kSHAPE_TYPE = ShapeType::ELLIPSE;
  static const bool kGIF = false;

  Approximator(const Image& target) :
    model_(target, Utils::AverageColor(target)),
    thread_pool_(kN_THREADS) {
  }

  void Run() {
    for (int i = 0; i < kN_SHAPES; ++i) {
      std::cout << i << ": " << model_.GetEnergy() << std::endl;
      MultiThreadingBestHillClimb(model_, kSHAPE_TYPE, kALPHA, kN_CLIMBS, kMAX_CLIMBS, kN_RANDOM_STATE);
      if (kGIF) {
       model_.Export(kPATH_GIF + std::to_string(i) + ".bmp");
      }
    }
    model_.Export(kPATH);
  }
};