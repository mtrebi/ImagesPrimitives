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
    long long r_diff = 0,
      g_diff = 0,
      b_diff = 0;
    for (int x = 0; x < target.GetWidth(); ++x) {
      for (int y = 0; y < target.GetHeight(); ++y) {
        const RGBApixel target_color = target.GetPixel(x, y);
        const RGBApixel current_color = current.GetPixel(x, y);

        // Calculate diff between target value and current value and rise to second power to dlete negatives
        r_diff += pow((current_color.Red - target_color.Red), 2);
        g_diff += pow((current_color.Green - target_color.Green), 2);
        b_diff += pow((current_color.Blue - target_color.Blue), 2);
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
 
  Triangle RandomShape() const {
    // TODO More Shapes
    Triangle triangle = Triangle(generator_.RandomPoint(), generator_.RandomPoint(), generator_.RandomPoint());
    while (!triangle.Valid()) {
      triangle = Triangle(generator_.RandomPoint(), generator_.RandomPoint(), generator_.RandomPoint());
    }

    triangle.SetColor(ShapeColor(triangle));
    return triangle;
  }

  Triangle BestRandomShape() const {
    // TODO Use numeric limits
    Triangle best_shape = RandomShape();
    float best_score = Score(target_, image_, best_shape);
    //std::cout << "\tFR-Shape score: " << best_score << std::endl;

    for (int i = 0; i < BEST_OF_RANDOM_SHAPES; ++i) {
      const Triangle new_shape = RandomShape();
      const float new_score = Score(target_, image_, new_shape);

      if (new_score < best_score) {
        best_shape = new_shape;
        best_score = new_score;
      }
    }
    //std::cout << "\tBR-Shape score: " << best_score << std::endl;
    return best_shape;
  }


  Triangle Mutate(Triangle& original) const {
    Triangle mutation = original.Mutate();
    mutation.SetColor(ShapeColor(mutation));
    return mutation;
  }

  RGBApixel ShapeColor(const Triangle& shape) const {
    const BoundingBox bbox = shape.BBox();

    long long r = 0,
      g = 0,
      b = 0;
    long count = 0;
    for (int x = bbox.min.x; x <= bbox.max.x; ++x) {
      for (int y = bbox.min.y; y <= bbox.max.y; ++y) {
        if (shape.Contains(Point(x, y))) {
          const RGBApixel color = target_.GetPixel(x, y);
          r += color.Red;
          g += color.Green;
          b += color.Blue;
          ++count;
        }
      }
    }

    float area = shape.Area();

    RGBApixel color;
    color.Red = r / count;
    color.Green = g / count;
    color.Blue = b / count;

    return color;
  }

  Triangle BestMutation(const Triangle& shape) const {
    // TODO Use numeric limits
    float max_score = Score(target_, image_, shape);
    Triangle previous_mutation = shape;
    for (int i = 0; i < N_MUTATIONS; ++i) {
      Triangle current_mutation = Mutate(previous_mutation);
      float new_score = Score(target_, image_, current_mutation);
      if (new_score < max_score) {
        previous_mutation = current_mutation;
        max_score = new_score;
      }
    }
    //std::cout << "\tM-Shape score: " << max_score << std::endl;
    return previous_mutation;
  }

  Triangle BestHillClimbing() const {
    // TODO Use numeric limits
    Triangle best_shape = BestMutation(BestRandomShape());
    float best_score = Score(target_, image_, best_shape);
    std::cout << "\tFH-Shape score: " << best_score << std::endl;
    for (int i = 0; i < N_HCLIMBS; ++i) {
      Triangle new_shape = BestMutation(BestRandomShape());
      const float new_score = Score(target_, image_, new_shape);

      if (new_score < best_score) {
        best_shape = new_shape;
        best_score = new_score;
      }
    }
    std::cout << "\tBH-Shape score: " << best_score << std::endl;

    return best_shape;
  }

  void Init() {
    image_.SetBaseColor(target_.AverageColor());
  }
  // TODO Optimizacion copiar solo bounding box de pixeles
  // TODO Optimizacion bounding box score parcial
  // TODO Threading
public:
  const int N_SHAPES = 10;
  const int BEST_OF_RANDOM_SHAPES = 50;
  const int N_MUTATIONS = 100;
  const int N_HCLIMBS = 10;

  Approximator(const Image& target) :
    image_(target.GetWidth(), target.GetHeight(), target.GetBitDepth(), target.GetHDPI(), target.GetVDPI()),
    generator_(target.GetWidth(), target.GetHeight()),
    target_ (target){
  }

  Image Run() {
    Init();
    // TODO Random - restart hill climbing
    //test();
    for (int i = 0; i < N_SHAPES; ++i) {
      std::cout << "Shape " << i << std::endl;
      Triangle best_triangle = BestHillClimbing();
      image_.AddShape(best_triangle);
    }
    return image_;
  }

  float test(const Image& a, const Image&b) {
    return Score(a, b);
  }
};
