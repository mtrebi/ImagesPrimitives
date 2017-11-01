#pragma once
#include "Shape.h"
#include <memory>
#include "Triangle.h"
enum class ShapeType {
  TRIANGLE,
  ELLIPSE
};

struct State;

struct State {
  std::shared_ptr<Shape> shape;
  float energy;
  int alpha;

  State() {}

  State(std::shared_ptr<Shape> sh, const float sc, const int a) {
    shape = sh;
    energy = sc;
    alpha = a;
  }

  State(const State& state) {
    this->shape = state.shape->Copy();
    this->energy = state.energy;
    this->alpha = state.alpha;
  }

  State Mutate() {
    State old_state(*this);
    shape->Mutate();
    energy = -1;
    return old_state;
  }
};