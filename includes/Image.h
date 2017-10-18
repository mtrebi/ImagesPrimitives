#pragma once
#include "../lib/EasyBMP_1.06/EasyBMP.h"


class Image {
private:
  BMP image_;
public:
  Image(const std::string image_path) {
    image_.ReadFromFile(image_path.c_str());
  }

  Image(const int width, const int height, const int bitdepth, const int hDPI, const int vDPI) {
    image_.SetSize(width, height);
    image_.SetBitDepth(bitdepth);
    image_.SetDPI(hDPI, vDPI);
  }

  bool Export(const std::string path) {
    return image_.WriteToFile(path.c_str());
  }

  RGBApixel AverageColor() const {
    long r = 0,
          g = 0,
          b = 0;
    for (int i = 0; i < image_.TellWidth(); ++i) {
      for (int j = 0; j < image_.TellHeight(); ++j) {
        const RGBApixel color = image_.GetPixel(i, j);
        r += color.Red;
        g += color.Green;
        b += color.Blue;
      }
    }

    RGBApixel average_color;
    average_color.Red = r / (image_.TellWidth() * image_.TellHeight());
    average_color.Green = g / (image_.TellWidth() * image_.TellHeight());
    average_color.Blue = b / (image_.TellWidth() * image_.TellHeight());

    return average_color;
  }

  void SetBaseColor(const RGBApixel color) {
    for (int i = 0; i < image_.TellWidth(); ++i) {
      for (int j = 0; j < image_.TellHeight(); ++j) {
        image_.SetPixel(i, j, color);
      }
    }
  }

};
