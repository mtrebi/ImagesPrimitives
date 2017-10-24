#pragma once
#include "../lib/EasyBMP_1.06/EasyBMP.h"
#include <memory>
#include "Shape.h"

class Image {
private:
  BMP bmp_;
public:
  Image(const std::string image_path) {
    bmp_.ReadFromFile(image_path.c_str());
  }

  Image(const int width, const int height, const int bitdepth, const int hDPI, const int vDPI) {
    bmp_.SetSize(width, height);
    bmp_.SetBitDepth(bitdepth);
    bmp_.SetDPI(hDPI, vDPI);
  }
  
  Image(const Image& image) {
    bmp_ = image.bmp_;
  }

  Image& operator=(const Image& image)  {
    bmp_ = image.bmp_;
    return *this;
  }

  Image(Image&& image) {
    bmp_ = std::move(image.bmp_);
    int a = 2;
  }

  Image& operator=(Image&& image) {
    bmp_ = std::move(image.bmp_);
    return *this;
  }


  bool Export(const std::string path) { 
    return bmp_.WriteToFile(path.c_str()); 
  }

  RGBApixel GetPixel(const int i, const int j) const { return bmp_.GetPixel(i, j); }
  int GetWidth() const { return bmp_.TellWidth(); }
  int GetHeight() const { return bmp_.TellHeight(); }
  int GetBitDepth() const { return bmp_.TellBitDepth(); }
  int GetVDPI() const { return bmp_.TellVerticalDPI(); }
  int GetHDPI() const { return bmp_.TellHorizontalDPI(); }
  int GetSize() const { return bmp_.TellWidth() * bmp_.TellHeight(); }


  void SetPixel(const int x, const int y, const RGBApixel& color) {
    bmp_.SetPixel(x, y, color);
  }

  RGBApixel AverageColor() const {
    long long r = 0,
      g = 0,
      b = 0;
    for (int i = 0; i < bmp_.TellWidth(); ++i) {
      for (int j = 0; j < bmp_.TellHeight(); ++j) {
        const RGBApixel color = bmp_.GetPixel(i, j);
        r += color.Red;
        g += color.Green;
        b += color.Blue;
      }
    }

    RGBApixel average_color;
    average_color.Red = r / GetSize();
    average_color.Green = g / GetSize();
    average_color.Blue = b / GetSize();
    average_color.Alpha = 255;

    return average_color;
  }

  void SetBaseColor(const RGBApixel& color) {
    for (int i = 0; i < bmp_.TellWidth(); ++i) {
      for (int j = 0; j < bmp_.TellHeight(); ++j) {
        bmp_.SetPixel(i, j, color);
      }
    }
  }

  void AddShape(const std::shared_ptr<Shape> shape) {
    const BoundingBox bbox = shape->GetBBox();
    for (int x = bbox.min.x; x < bbox.max.x; ++x) {
      for (int y = bbox.min.y; y < bbox.max.y; ++y) {
        if (shape->Contains(Point(x, y))) {
          RGBApixel current_color = this->GetPixel(x, y);
          const float alpha = shape->GetColor().Alpha / 255.0f;
          const float one_minus_alpha = 1 - alpha;

          RGBApixel new_color;
          new_color.Red = shape->GetColor().Red * alpha + one_minus_alpha * current_color.Red;
          new_color.Green = shape->GetColor().Green * alpha + one_minus_alpha * current_color.Green;
          new_color.Red = shape->GetColor().Blue * alpha + one_minus_alpha * current_color.Blue;
          new_color.Alpha = shape->GetColor().Alpha;
          this->SetPixel(x, y, new_color);
        }
      }
    }
  }

};
