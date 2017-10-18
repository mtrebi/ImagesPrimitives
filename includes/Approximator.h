#pragma once
#include "Image.h"
#include "Triangle.h"
#include "RandomGenerator.h"

class Approximator {
private:
  Image image_,
    target_;
  RandomGenerator generator_;
  float max_score_ = 0;

  // Score calculation using root-mean-square-error
  float Score(const Image& target, const Image& current) const {
    long r_diff = 0,
      g_diff = 0,
      b_diff = 0;
    for (int i = 0; i < target.GetWidth(); ++i) {
      for (int j = 0; j < target.GetHeight(); ++j) {
        const RGBApixel target_color = target.GetPixel(i, j);
        const RGBApixel current_color = current.GetPixel(i, j);

        // Calculate diff between target value and current value and rise to second power to dlete negatives
        r_diff += pow((target_color.Red - current_color.Red), 2);
        g_diff += pow((target_color.Green - current_color.Green), 2);
        b_diff += pow((target_color.Blue - current_color.Blue), 2);
      }
    }

    // Divide by the number of samples
    r_diff /= (target.GetSize());
    g_diff /= (target.GetSize());
    b_diff /= (target.GetSize());

    // Calculate sqrt
    r_diff = sqrt(r_diff);
    g_diff = sqrt(g_diff);
    b_diff = sqrt(b_diff);

    // Average between tree colors components
    return (r_diff + g_diff + b_diff) / 3;
  }

  float Score(const Image& target, Image current, const Triangle shape) const {
    current.AddShape(shape);
    return Score(target, current);
  }
 
  Triangle GenerateShape() const {
    // TODO More Shapes
    return Triangle(generator_.RandomPoint(), 
                    generator_.RandomPoint(),
                    generator_.RandomPoint());
  }

  Triangle MutateShape(const Triangle shape) const {
    Image copy = Image(image_);
    float max_score = Score(target_, copy, shape);

    Triangle previous_mutation = shape;
    for (int i = 0; i < N_MUTATIONS; ++i) {
      Triangle current_mutation = previous_mutation.Mutate();
      float new_score = Score(target_, copy, current_mutation);
      if (new_score > max_score) {
        previous_mutation = current_mutation;
        max_score = new_score;
      }
    }
  }

public:
  const int N_SHAPES = 50;
  const int N_MUTATIONS = 15;

  Approximator(const Image& target) :
    image_(target.GetWidth(), target.GetHeight(), target.GetBitDepth(), target.GetHDPI(), target.GetVDPI()),
    generator_(target.GetWidth(), target.GetHeight()),
    target_ (target){

    image_.SetBaseColor(target.AverageColor());
  }

  Image Run() {
    // TODO Random - restart hill climbing
    for (int i = 0; i < N_SHAPES; ++i) {
      Triangle shape = GenerateShape();
      Triangle mutated_shape = MutateShape(shape);

      image_.AddShape(mutated_shape);
    }

    return image_;
  }

};
