#include <iostream>
#include "ImageUtils.h"

int main() {
  BMP target_image = ImageUtils::Read("../assets/monalisa.bmp");
  const RGBApixel average_color = ImageUtils::AverageColor(target_image);


  BMP current_image = ImageUtils::New(target_image.TellWidth(), target_image.TellHeight(), target_image.TellBitDepth(), target_image.TellHorizontalDPI(), target_image.TellVerticalDPI());
  ImageUtils::SetBaseColor(current_image, average_color);

  ImageUtils::Write(current_image, "monalisa_out.bmp");

  return 0;
}

