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
                      b_diff * b_diff/* + 
                      a_diff * a_diff*/;
      }
    }

    return sqrt(total_diff / (target.GetSize() * 3)) / 255;
  }

  // Score calculation using root-mean-square-error
  static float Energy(const Image& target, Image current, const std::shared_ptr<Shape> shape) {
    Utils::AddShape(current, shape);
    shape->SetEnergy(Energy(target, current));
    return shape->GetEnergy();
  }


  static void AddShape(Image& image, const std::shared_ptr<Shape> shape) {
    int sr, sg, sb, sa;
    Utils::ScaleByAlpha(sr, sg, sb, sa, shape->GetColor());

    const int m = 0xFFFF;
    const int ma = 0xFFFF;
    const float as = (m - (sa * (ma / m))) * 257;
    const int a = as;

    const BoundingBox bbox = shape->GetBBox();
    for (int x = bbox.min.x; x < bbox.max.x; ++x) {
      for (int y = bbox.min.y; y < bbox.max.y; ++y) {
        if (shape->Contains(Point(x, y))) {
          RGBApixel d = image.GetPixel(x, y);

          RGBApixel new_color;
          new_color.Red =   ((d.Red   * a + sr * ma) / m) >> 8;
          new_color.Green = ((d.Green * a + sg * ma) / m) >> 8;
          new_color.Blue =  ((d.Blue  * a + sb * ma) / m) >> 8;
          new_color.Alpha = ((d.Alpha * a + sa * ma) / m) >> 8;

          image.SetPixel(x, y, new_color);
        }
      }
    }
  }

  // Scale the rgb color components by the alpha component
  static void ScaleByAlpha(int& sr, int& sg, int& sb, int& sa, const RGBApixel &c) {
    sr = c.Red | (c.Red << 8);
    sr *= c.Alpha;
    sr /= 0xFF;

    sg = c.Green | (c.Green << 8);
    sg *= c.Alpha;
    sg /= 0xFF;

    sb = c.Blue | (c.Blue << 8);
    sb *= c.Alpha;
    sb /= 0xFF;

    sa = c.Alpha | (c.Alpha << 8);
  }
};