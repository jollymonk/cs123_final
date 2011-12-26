#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"

class Cylinder : public Shape {

public:
    Cylinder();

    virtual ~Cylinder();

    void createGeometry();

    REAL checkIntersection(Vector4 p_eye, Vector4 v_dir);

    Vector4 getNormal(Vector4 p_intersect_obj);

    Vector2 getUV(Vector4 p_intersect_obj);

};

#endif // CYLINDER_H
