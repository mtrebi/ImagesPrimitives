#pragma once
#include "Image.h"
class Shape;
#include "../lib/EasyBMP_1.06/EasyBMP.h"
#include <memory>

static class Utils {
public:
  static int Clamp(const int value, const int min, const int max) {
    if (value < min) {
      return min;
    }

    if (value > max) {
      return max;
    }

    return value;
  }

  // Score calculation using root-mean-square-error
  static float Energy(const Image& target, const Image& current)  {
    long long total_diff = 0;

    for (int x = 0; x < target.GetWidth(); ++x) {
      for (int y = 0; y < target.GetHeight(); ++y) {
        const RGBApixel target_color = target.GetPixel(x, y);
        const RGBApixel current_color = current.GetPixel(x, y);

        // Calculate diff between target value and current value and rise to second power to dlete negatives
        const long long r_diff = (current_color.Red - target_color.Red);
        const long long g_diff = (current_color.Green - target_color.Green);
        const long long b_diff = (current_color.Blue - target_color.Blue);
        const long long a_diff = (current_color.Alpha - target_color.Alpha);

        total_diff += r_diff * r_diff + 
                      g_diff * g_diff + 
                      b_diff * b_diff + 
                      a_diff * a_diff;
      }
    }

    return sqrt(total_diff / (target.GetSize() * 4)) / 255;
  }

  // Score calculation using root-mean-square-error
  static float Energy(const Image& target, Image current, const std::shared_ptr<Shape> shape) {
    current.AddShape(shape);
    shape->SetEnergy(Energy(target, current));
    return shape->GetEnergy();
  }

};