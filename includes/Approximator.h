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
  
  void MultiThreadingBestHillClimb(Model& model, const ShapeType& shape_type, const int alpha, const int n_climbs, const int max_climbs, const int n_random_state, const int max_random) {
    std::vector<std::future<State>> results;
    for (int i = 0; i < N_THREADS; ++i) {
      RandomGenerator generator(i);
      results.push_back(thread_pool_.push(std::bind(&Model::BestHillClimb, std::ref(model_), std::ref(generator), shape_type, alpha, n_climbs, max_climbs, n_random_state, max_random)));
    }

    State best_state, state;
    for (int i = 0; i < N_THREADS; ++i) {
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
  std::string OUTPUT_PATH = "";
  ShapeType SHAPE_TYPE;

  int N_THREADS;
  int N_RANDOM_STATE;
  int N_CLIMBS;
  int MAX_CLIMBS;
  int MAX_RANDOM;
  int ALPHA;
  int N_SHAPES;

  bool GIF;

  Approximator(const Image& target, const int n_threads) :
    model_(target, Utils::AverageColor(target)),
    N_THREADS(n_threads),
    thread_pool_(n_threads) {
  }

  void Run() {
    std::cout << "Starting..." << std::endl;

    for (int i = 0; i < N_SHAPES; ++i) {
      std::cout << i << ": " << model_.GetEnergy() << std::endl;
      MultiThreadingBestHillClimb(model_, SHAPE_TYPE, ALPHA, N_CLIMBS, MAX_CLIMBS, N_RANDOM_STATE, MAX_RANDOM);
      if (GIF) {
        std::string gif_path = OUTPUT_PATH;
        model_.Export(gif_path.insert(gif_path.length() - 4, std::to_string(i)));
      }
    }
    model_.Export(OUTPUT_PATH);
    std::cout << "Output stored at "  + OUTPUT_PATH << std::endl;
    std::cout << "Finishing..." << std::endl;
  }
};