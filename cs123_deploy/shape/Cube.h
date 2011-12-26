#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

class Cube : public Shape {

public:
    Cube();

    virtual ~Cube();

    void createGeometry();

    REAL checkIntersection(Vector4 p_eye, Vector4 v_dir);

    Vector4 getNormal(Vector4 p_intersect_obj);

    Vector2 getUV(Vector4 p_intersect_obj);
};

#endif // CUBE_H
