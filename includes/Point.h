#ifndef __POINT_2D__
#define __POINT_2D__


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// 2D points are used to store sample points

#include "Vector.h"

class Point {
public:

  int x, y;

public:

  Point(void);
  Point(const int arg);
  Point(const int x1, const int y1);
  Point(const Point& p);
  ~Point(void);

  Point& 													// assignment operator
    operator= (const Point& p);

  Point 													// unary minus
    operator- (void) const;

  Vector 													// vector joining two points
    operator- (const Point& p) const;

  Point 													// addition of a vector				
    operator+ (const Vector& v) const;

  Point 													// subtraction of a vector
    operator- (const Vector& v) const;

  Point 													// multiplication by a double on the right
    operator* (const double a) const;

  double														// square of distance bertween two points
    d_squared(const Point& p) const;

  double														// distance bewteen two points
    distance(const Point& p) const;
};


// inlined member functions

// -------------------------------------------------------------- operator-
// unary minus

inline Point
Point::operator- (void) const {
  return (Point(-x, -y));
}


// -------------------------------------------------------------- operator-
// the vector that joins two points

inline Vector
Point::operator- (const Point& p) const {
  return (Vector(x - p.x, y - p.y));
}


// -------------------------------------------------------------- operator+
// addition of a vector to a point that returns a new point

inline Point
Point::operator+ (const Vector& v) const {
  return (Point(x + v.x, y + v.y));
}


// -------------------------------------------------------------- operator-
// subtraction of a vector from a point that returns a new point

inline Point
Point::operator- (const Vector& v) const {
  return (Point(x - v.x, y - v.y));
}


// -------------------------------------------------------------- operator*
// mutliplication by a double on the right

inline Point
Point::operator* (const double a) const {
  return (Point(x * a, y * a));
}


// -------------------------------------------------------------- d_squared
// the square of the distance between the two points as a member function

inline double
Point::d_squared(const Point& p) const {
  return ((x - p.x) * (x - p.x)
    + (y - p.y) * (y - p.y));
}




// inlined non-member function

// -------------------------------------------------------------- operator*
// multiplication by a double on the left

Point												// prototype
operator* (double a, const Point& p);

inline Point
operator* (double a, const Point& p) {
  return (Point(a * p.x, a * p.y));
}

#endif

