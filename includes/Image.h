#pragma once
#include "../lib/EasyBMP_1.06/EasyBMP.h"
#include <memory>

class Image {
private:
  BMP bmp_;
public:
  Image(const std::string image_path) {
    bmp_.ReadFromFile(image_path.c_str());
  }

  Image(const RGBApixel& background_color, const int width, const int height, const int bitdepth, const int hDPI, const int vDPI) {
    bmp_.SetSize(width, height);
    bmp_.SetBitDepth(bitdepth);
    bmp_.SetDPI(hDPI, vDPI);

    for (int i = 0; i < bmp_.TellWidth(); ++i) {
      for (int j = 0; j < bmp_.TellHeight(); ++j) {
        bmp_.SetPixel(i, j, background_color);
      }
    }
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

  bool Export(const std::string path) {     return bmp_.WriteToFile(path.c_str()); }

  int GetWidth() const { return bmp_.TellWidth(); }
  int GetHeight() const { return bmp_.TellHeight(); }
  int GetBitDepth() const { return bmp_.TellBitDepth(); }
  int GetVDPI() const { return bmp_.TellVerticalDPI(); }
  int GetHDPI() const { return bmp_.TellHorizontalDPI(); }
  int GetSize() const { return bmp_.TellWidth() * bmp_.TellHeight(); }

  RGBApixel& GetPixel(const int i, const int j) const { return bmp_.GetPixel(i, j); }
  void SetPixel(const int x, const int y, const RGBApixel& color) { bmp_.SetPixel(x, y, color); }
};
