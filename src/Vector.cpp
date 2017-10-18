// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// This file contains the definition of the class Vector

#include <math.h>    // for sqrt

#include "Vector.h"
#include "Point.h"

// ---------------------------------------------------------- default constructor

Vector::Vector(void)
  : x(0.0), y(0.0)
{}

// ---------------------------------------------------------- constructor

Vector::Vector(double a)
  : x(a), y(a)
{}

// ---------------------------------------------------------- constructor

Vector::Vector(double _x, double _y)
  : x(_x), y(_y)
{}

// ---------------------------------------------------------- copy constructor

Vector::Vector(const Vector& vector)
  : x(vector.x), y(vector.y)
{}


Vector::Vector(const Point& p)
  : x(p.x), y(p.y)
{}


// ---------------------------------------------------------- destructor

Vector::~Vector(void) {}



// ---------------------------------------------------------- assignment operator

Vector&
Vector::operator= (const Vector& rhs) {
  if (this == &rhs)
    return (*this);

  x = rhs.x; y = rhs.y;

  return (*this);
}


// ---------------------------------------------------------- assignment operator 
// assign a point to a vector

Vector&
Vector::operator= (const Point& rhs) {
  x = rhs.x; y = rhs.y;
  return (*this);
}


// ----------------------------------------------------------  length
// length of the vector

double
Vector::length(void) {
  return (sqrt(x * x + y * y));
}


// ----------------------------------------------------------  normalize
// converts the vector to a unit vector

void
Vector::normalize(void) {
  double length = sqrt(x * x + y * y);
  x /= length; y /= length;
}


// ----------------------------------------------------------  hat
// converts the vector to a unit vector and returns the vector

Vector&
Vector::hat(void) {
  double length = sqrt(x * x + y * y);
  x /= length; y /= length;
  return (*this);
}