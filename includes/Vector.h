#ifndef __VECTOR_2D__
#define __VECTOR_2D__


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


class Point;

//----------------------------------------- class Vector2D

class Vector {
public:

  double	x, y;

public:

  Vector(void);											// default constructor
  Vector(double a);										// constructor
  Vector(double _x, double _y);				// constructor
  Vector(const Vector& v);							// copy constructor
  Vector(const Point& p);								// constructs a vector from a point

  ~Vector(void);										// destructor

  Vector& 												// assignment operator
    operator= (const Vector& rhs);

  Vector& 												// assign a Point2D to a vector
    operator= (const Point& rhs);

  Vector												// unary minus
    operator- (void) const;

  double													// length
    length(void);

  double													// square of the length
    len_squared(void);

  Vector												// multiplication by a double on the right
    operator* (const double a) const;

  Vector												// division by a double
    operator/ (const double a) const;

  Vector												// addition
    operator+ (const Vector& v) const;

  Vector& 												// compound addition
    operator+= (const Vector& v);

  Vector												// subtraction
    operator- (const Vector& v) const;

  double 													// dot product							
    operator* (const Vector& b) const;

  Vector 												// cross product				
    operator^ (const Vector& v) const;

  void 													// convert vector to a unit vector
    normalize(void);

  Vector& 												// return a unit vector, and normalize the vector												
    hat(void);
};


// inlined member functions

// ------------------------------------------------------------------------ unary minus
// this does not change the current vector
// this allows ShadeRec objects to be declared as constant arguments in many shading
// functions that reverse the direction of a ray that's stored in the ShadeRec object

inline Vector
Vector::operator- (void) const {
  return (Vector(-x, -y));
}


// ---------------------------------------------------------------------  len_squared
// the square of the length

inline double
Vector::len_squared(void) {
  return (x * x + y * y);
}


// ----------------------------------------------------------------------- operator*
// multiplication by a double on the right

inline Vector
Vector::operator* (const double a) const {
  return (Vector(x * a, y * a));
}

// ----------------------------------------------------------------------- operator/
// division by a double

inline Vector
Vector::operator/ (const double a) const {
  return (Vector(x / a, y / a));
}


// ----------------------------------------------------------------------- operator+
// addition

inline Vector
Vector::operator+ (const Vector& v) const {
  return (Vector(x + v.x, y + v.y));
}


// ----------------------------------------------------------------------- operator-
// subtraction

inline Vector
Vector::operator- (const Vector& v) const {
  return (Vector(x - v.x, y - v.y));
}


// ----------------------------------------------------------------------- operator*
// dot product

inline double
Vector::operator* (const Vector& v) const {
  return (x * v.x + y * v.y);
}


// ----------------------------------------------------------------------- operator^
// cross product

inline Vector
Vector::operator^ (const Vector& v) const {
  return (Vector(x * v.y - y * v.x));
}


// ---------------------------------------------------------------------  operator+=
// compound addition

inline Vector&
Vector::operator+= (const Vector& v) {
  x += v.x; y += v.y;
  return (*this);
}




// inlined non-member function

// ----------------------------------------------------------------------- operator*
// multiplication by a double on the left

Vector 											// prototype
operator* (const double a, const Vector& v);

inline Vector
operator* (const double a, const Vector& v) {
  return (Vector(a * v.x, a * v.y));
}

#endif



