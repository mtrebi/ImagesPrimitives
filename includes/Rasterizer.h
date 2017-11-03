#pragma once

#include "Image.h"
#include "Scanline.h"

class Rasterizer {
public:
  static void DrawLines(Image& image, const RGBApixel& color, const std::vector<Scanline>& lines) {
    int sr = color.Red,
      sg = color.Green,
      sb = color.Blue,
      sa = color.Alpha;

    sr = sr | (color.Red << 8);
    sr *= color.Alpha;
    sr /= 0xFF;

    sg = sg | (color.Green << 8);
    sg *= color.Alpha;
    sg /= 0xFF;

    sb = sb | (color.Blue << 8);
    sb *= color.Alpha;
    sb /= 0xFF;

    sa = sa | (color.Alpha << 8);

    const int m = 0xFFFF;
    const int ma = 0xFFFF;
    const int a = (m - (sa * (ma / m))) * 257;

    for (const auto& line : lines) {
      for (int x = line.X1; x <= line.X2; ++x) {
        RGBApixel d = image.GetPixel(x, line.Y);

        RGBApixel new_color;
        new_color.Red = ((d.Red   * a + sr * ma) / m) >> 8;
        new_color.Green = ((d.Green * a + sg * ma) / m) >> 8;
        new_color.Blue = ((d.Blue  * a + sb * ma) / m) >> 8;
        new_color.Alpha = ((d.Alpha * a + sa * ma) / m) >> 8;
        image.SetPixel(x, line.Y, new_color);
      }
    }
  }

  static void CopyLines(Image& dest, const Image& source, const std::vector<Scanline>& lines) {
    for (const auto& line : lines) {
      for (int x = line.X1; x <= line.X2; ++x) {
        dest.SetPixel(x, line.Y, source.GetPixel(x, line.Y));
      }
    }
  }
};