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
};