#include "../lib/EasyBMP_1.06/EasyBMP.h"


class ImageUtils {
public:

  static BMP New(const int width, const int height, const int bitdepth, const int hDPI, const int vDPI) {
    BMP image;
    image.SetSize(width, height);
    image.SetBitDepth(bitdepth);
    image.SetDPI(hDPI, vDPI);
    return image;
  }

  static void SetBaseColor(BMP& image, const RGBApixel color) {
    for (int i = 0; i < image.TellWidth(); ++i) {
      for (int j = 0; j < image.TellHeight(); ++j) {
        image.SetPixel(i, j, color);
      }
    }
  }

  static RGBApixel AverageColor(BMP& image) {
    long r = 0,
          g = 0,
          b = 0;
    for (int i = 0; i < image.TellWidth(); ++i) {
      for (int j = 0; j < image.TellHeight(); ++j) {
        const RGBApixel color = image.GetPixel(i, j);
        r += color.Red;
        g += color.Green;
        b += color.Blue;
      }
    }

    RGBApixel average_color;
    average_color.Red = r / (image.TellWidth() * image.TellHeight());
    average_color.Green = g / (image.TellWidth() * image.TellHeight());
    average_color.Blue = b / (image.TellWidth() * image.TellHeight());

    return average_color;
  }

  static BMP Read(const std::string image_path) {
    BMP image;
    image.ReadFromFile(image_path.c_str());

    return image;
  }

  static bool Write(BMP& image, const std::string output_location) {
    return image.WriteToFile(output_location.c_str());
  }
};
