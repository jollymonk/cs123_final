/*!
   @file   CS123Algebra.h
   @author Travis Fischer (fisch0920@gmail.com)
   @date   Fall 2008
   
   @brief
      Convenience header which includes all headers in the CS123 linear 
   algebra package (CS123Matrix.h, CS123Vector.h, and CS123Point.h).
**/

#ifndef __CS123_ALGEBRA_H__
#define __CS123_ALGEBRA_H__

typedef double REAL;

#include <CS123Matrix.h>
#include <CS123Vector.h>

#ifndef M_PI
#define M_PI 3.14159265f
#endif

static double toDegrees(double const radians) { return (radians * (180.f/M_PI)); }
static double toRadians(double const degrees) { return (degrees * (M_PI/180.f)); }

#endif // __CS123_ALGEBRA_H__

