#pragma once

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
  static float Energy(const Image& target, const Image& current) const {
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

  // Score calculation using root-mean-square-error
  static float Energy(const Image& target, Image current, const Triangle shape) const {
    current.AddShape(shape);
    return Energy(target, current);
  }

};