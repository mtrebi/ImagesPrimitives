#include "Shape.h"
#include "Utils.h"

void Shape::ComputeColor(const Image& target, const Image& current, const int alpha) {
  const BoundingBox bbox = this->GetBBox();
  long long total_red = 0,
    total_green = 0,
    total_blue = 0;
  long long count = 0;

  const int a = 257 * 255 / alpha;

  for (int x = bbox.min.x; x <= bbox.max.x; ++x) {
    for (int y = bbox.min.y; y <= bbox.max.y; ++y) {
      if (this->Contains(Point(x, y))) {
        const RGBApixel target_color = target.GetPixel(x, y);
        const RGBApixel current_color = current.GetPixel(x, y);

        // Mix the red, green and blue components, blending by the given alpha value
        total_red += (target_color.Red - current_color.Red) * a + current_color.Red * 257;
        total_green += (target_color.Green - current_color.Green) * a + current_color.Green * 257;
        total_blue += (target_color.Blue - current_color.Blue) * a + current_color.Blue * 257;
        ++count;
      }
    }
  }

  color_.Red = Utils::Clamp((total_red / count) >> 8, 0, 255);
  color_.Green = Utils::Clamp((total_green / count) >> 8, 0, 255);
  color_.Blue = Utils::Clamp((total_blue / count) >> 8, 0, 255);
  color_.Alpha = alpha;
}