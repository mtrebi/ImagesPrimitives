#pragma once
#include "Image.h"
#include "../lib/EasyBMP_1.06/EasyBMP.h"
#include <memory>
#include <vector>
#include "Scanline.h"
static class Utils {
public:

  static void Swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
  }

  static RGBApixel AverageColor(const Image& image) {
    long long r = 0,
      g = 0,
      b = 0;
    for (int x = 0; x < image.GetWidth(); ++x) {
      for (int y = 0; y < image.GetHeight(); ++y) {
        const RGBApixel color = image.GetPixel(x, y);
        r += color.Red;
        g += color.Green;
        b += color.Blue;
      }
    }

    RGBApixel average_color;
    average_color.Red = r / (image.GetWidth() * image.GetHeight());
    average_color.Green = g / (image.GetWidth() * image.GetHeight());
    average_color.Blue = b / (image.GetWidth() * image.GetHeight());
    average_color.Alpha = 255;

    return average_color;
  }

  static int Clamp(const int value, const int min, const int max) {
    if (value <= min) {
      return min;
    }

    if (value >= max) {
      return max;
    }

    return value;
  }

  // Score calculation using root-mean-square-error
  static float DifferenceFull(const Image& target, const Image& current)  {
    long long total_diff = 0;

    for (int y = 0; y < target.GetHeight(); ++y) {
      for (int x = 0; x < target.GetWidth(); ++x) {
        const RGBApixel target_color = target.GetPixel(x, y);
        const RGBApixel current_color = current.GetPixel(x, y);

        // Calculate diff between target value and current value and rise to second power to dlete negatives
        const long long r_diff = (target_color.Red - current_color.Red);
        const long long g_diff = (target_color.Green - current_color.Green);
        const long long b_diff = (target_color.Blue - current_color.Blue);

        total_diff += r_diff * r_diff +
          g_diff * g_diff +
          b_diff * b_diff;
      }
    }

    return sqrt(total_diff / (target.GetSize() * 3.0f)) / 255;
  }

  static float DifferencePartial(const Image& target, const Image& before, const Image& after, const float score, const std::vector<Scanline>& lines) {
    long long total = pow(score * 255, 2) * target.GetHeight() * target.GetWidth() * 3;

    for (const auto& line : lines) {
      for (int x = line.X1; x <= line.X2; ++x) {
        const RGBApixel target_color = target.GetPixel(x, line.Y);
        const RGBApixel before_color = before.GetPixel(x, line.Y);
        const RGBApixel after_color = after.GetPixel(x, line.Y);

        long long dr1 = target_color.Red - before_color.Red;
        long long dg1 = target_color.Green - before_color.Green;
        long long db1 = target_color.Blue - before_color.Blue;

        long long dr2 = target_color.Red - after_color.Red;
        long long dg2 = target_color.Green - after_color.Green;
        long long db2 = target_color.Blue - after_color.Blue;

        total -= dr1*dr1 + dg1*dg1 + db1*db1;
        total += dr2*dr2 + dg2*dg2 + db2*db2;
      }
    }

    return sqrt(total / (target.GetSize() * 3.0f)) / 255;;
  }

};