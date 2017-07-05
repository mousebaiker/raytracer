#include "linal.h"

namespace raytracer {
#define RGB linal::Vector<int>
#define WHITE linal::Vector<int>(255, 255, 255)
#define BLACK linal::Vector<int>(0, 0, 0)

RGB operator*(double scalar, RGB v) {
  int red = (int)(scalar * (double)v.x());
  int green = (int)(scalar * (double)v.y());
  int blue = (int)(scalar * (double)v.z());
  return RGB(red, green, blue);
}
}  // namespace raytracer
