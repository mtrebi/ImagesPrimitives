#pragma once
#include "Image.h"
#include "Shape.h"
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
    long long r_diff = 0,
      g_diff = 0,
      b_diff = 0,
      a_diff = 0,
      total_diff = 0;

    for (int x = 0; x < target.GetWidth(); ++x) {
      for (int y = 0; y < target.GetHeight(); ++y) {
        const RGBApixel target_color = target.GetPixel(x, y);
        const RGBApixel current_color = current.GetPixel(x, y);

        // Calculate diff between target value and current value and rise to second power to dlete negatives
        r_diff += pow((current_color.Red - target_color.Red), 2);
        g_diff += pow((current_color.Green - target_color.Green), 2);
        b_diff += pow((current_color.Blue - target_color.Blue), 2);
        a_diff += pow((current_color.Alpha - target_color.Alpha), 2);

        total_diff += r_diff + g_diff + b_diff + a_diff;
      }
    }

    return sqrt(total_diff / (target.GetSize() * 4) / 255);
  }

  // Score calculation using root-mean-square-error
  static float Energy(const Image& target, Image current, const std::shared_ptr<Shape> shape) {
    shape->AddToImage(current);
    return Energy(target, current);
  }

};