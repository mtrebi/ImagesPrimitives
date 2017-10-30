#pragma once

#include <vector>

struct Scanline {
  int Y, X1, X2;

  Scanline(const int y, const int x1, const int x2)
    : Y(y), X1(x1), X2(x2) {

  }

  Scanline(const Scanline& s)
    : Y(s.Y), X1(s.X1), X2(s.X2) {

  } 
};