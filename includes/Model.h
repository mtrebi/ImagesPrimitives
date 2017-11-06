#pragma once
#include "Image.h"
#include "Point.h"
#include "State.h"
#include "Triangle.h"
#include "Ellipse.h"
#include "Utils.h"

#include "Rasterizer.h"

#include <array>
#include <string>
#include <memory>

class Model {
private:
  Image target_, current_, buffer_;
  RGBApixel background_color_;
  float energy_;

  State BestRandomState(RandomGenerator& generator, const ShapeType& shape_type, const int n_random_state, const int alpha, const int max_random) {
    State best_state, state;

    for (int i = 0; i < n_random_state; ++i) {
      state = RandomState(generator, shape_type, alpha, max_random);
      if (i == 0 || state.energy < best_state.energy) {
        best_state = state;
      }
    }
    return best_state;
  }

  State RandomState(RandomGenerator& generator, const ShapeType& shape_type, const int alpha, const int max_random) {
    State state;
    state.alpha = alpha;

    switch (shape_type) {
    case ShapeType::TRIANGLE:
      state.shape = std::shared_ptr<Shape>(new Triangle(generator, target_.GetWidth(), target_.GetHeight(), max_random));
      break;
    case ShapeType::ELLIPSE:
      state.shape = std::shared_ptr<Shape>(new Ellipse(generator, target_.GetWidth(), target_.GetHeight(), max_random));
      break;
    }

    state.energy = Energy(state.shape, alpha);
    return state;
  }

  float Energy(const std::shared_ptr<Shape> shape, const int alpha) {
    std::vector<Scanline> lines = shape->Rasterize();
    const RGBApixel color = ComputeColor(target_, current_, lines, alpha);
    Rasterizer::CopyLines(buffer_, current_, lines);
    Rasterizer::DrawLines(buffer_, color, lines);
    return Utils::DifferencePartial(target_, current_, buffer_, energy_, lines);
  }
  
  
  RGBApixel ComputeColor(const Image& target, const Image& current, const std::vector<Scanline>& lines, const int alpha) const {
    long long total_red = 0,
      total_green = 0,
      total_blue = 0;
    long long count = 0;

    const int a = 257 * 255 / alpha;

    for (const auto& line : lines) {
      for (int x = line.X1; x <= line.X2; ++x) {
        const RGBApixel target_color = target.GetPixel(x, line.Y);
        const RGBApixel current_color = current.GetPixel(x, line.Y);

        // Mix the red, green and blue components, blending by the given alpha value
        total_red += (target_color.Red - current_color.Red) * a + current_color.Red * 257;
        total_green += (target_color.Green - current_color.Green) * a + current_color.Green * 257;
        total_blue += (target_color.Blue - current_color.Blue) * a + current_color.Blue * 257;
        ++count;
      }
    }
    RGBApixel color;

    if (count == 0) {
      color.Red = 0;
      color.Green = 0;
      color.Blue = 0;
      color.Alpha = 0;

      return color;
    }

    color.Red = Utils::Clamp((total_red / count) >> 8, 0, 255);
    color.Green = Utils::Clamp((total_green / count) >> 8, 0, 255);
    color.Blue = Utils::Clamp((total_blue / count) >> 8, 0, 255);
    color.Alpha = alpha;

    return color;
  }

  State HillClimb(const State& s, const int n_climbs, const int max_climbs) {
    State state = State(s);
    State best_state = State(s);
    int n_mutations = 0;
    for (int i = 0; i < n_climbs && n_mutations < max_climbs; ++i) {
      State previous = state.Mutate();
      state.energy = Energy(state.shape, state.alpha);
      
      if (state.energy >= best_state.energy) {
        state = previous;
      }
      else {
        best_state = State(state);
        i = -1;
      }
      ++n_mutations;
    }
    return best_state;
  }

public:
  Model(const Image& target, const RGBApixel& background)
    : target_(target),
      background_color_(background),
      buffer_(background, target.GetWidth(), target.GetHeight(), target.GetBitDepth(), target.GetHDPI(), target.GetVDPI()),
      current_(background, target.GetWidth(), target.GetHeight(), target.GetBitDepth(), target.GetHDPI(), target.GetVDPI()),
      energy_(Utils::DifferenceFull(target_, current_)){
  }

  float GetEnergy() const { return energy_; }
  bool Export(const std::string path) { return current_.Export(path); }

  State BestHillClimb(RandomGenerator& generator, const ShapeType& shape_type,const int alpha, const int n_climbs, const int max_climbs, const int n_random_state, const int max_random) {
    State best_random_state = BestRandomState(generator, shape_type, n_random_state, alpha, max_random);
    State best_hillclimb_state = HillClimb(best_random_state, n_climbs, max_climbs);
    std::cout << "\t" << n_random_state << "x random: " << best_random_state.energy << " -> " << n_climbs << "x hill climb: " << best_hillclimb_state.energy << std::endl;
    return best_hillclimb_state;
  }

  void Add(const std::shared_ptr<Shape> shape, const int alpha) {
    Image copy(current_);
    const std::vector<Scanline> lines = shape->Rasterize();
    const RGBApixel color = ComputeColor(target_, current_, lines, alpha);
    Rasterizer::DrawLines(current_, color, lines);
    energy_ = Utils::DifferencePartial(target_, copy, current_, energy_, lines);
  }
};
