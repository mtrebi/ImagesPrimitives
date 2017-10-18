// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Point.h"

// ------------------------------------------------ default constructor

Point::Point(void)
  : x(0.0), y(0.0)
{}


// ------------------------------------------------ constructor

Point::Point(const int arg)
  : x(arg), y(arg)
{}


// ------------------------------------------------ constructor

Point::Point(const int x1, const int y1)
  : x(x1), y(y1)
{}


// ------------------------------------------------ copy constructor

Point::Point(const Point& p)
  : x(p.x), y(p.y)
{}


// ------------------------------------------------ destructor

Point::~Point(void) {}


// ------------------------------------------------ assignment operator

Point&
Point::operator= (const Point& rhs) {
  if (this == &rhs)
    return (*this);

  x = rhs.x;
  y = rhs.y;

  return (*this);
}