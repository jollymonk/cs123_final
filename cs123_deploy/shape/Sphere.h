#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape {

public:
    Sphere();

    virtual ~Sphere();

    //! Overwrites Shape createGeometry method
   void createGeometry();

   Vector4 sphereCoord(double theta, double phi);

   Vector4 sphereNorm(double theta, double phi);

   REAL checkIntersection(Vector4 p_eye, Vector4 v_dir);

   Vector4 getNormal(Vector4 p_intersect_obj);

   Vector2 getUV(Vector4 p_intersect_obj);

};

#endif // SPHERE_H
