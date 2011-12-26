#ifndef CONE_H
#define CONE_H

#include "Shape.h"

class Cone : public Shape {

public:
    Cone();

    virtual ~Cone();

    void createGeometry();

    Vector4 coneNorm(double theta);

    REAL checkIntersection(Vector4 p_eye, Vector4 v_dir);

    Vector4 getNormal(Vector4 p_intersect_obj);

    Vector2 getUV(Vector4 p_intersect_obj);
};

#endif // CONE_H
