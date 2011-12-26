#ifndef SHAPE_H
#define SHAPE_H

#include <CS123Common.h>
#include <qgl.h>
#include "math/CS123Algebra.h"
#include "Settings.h"

#define PI 3.14159265
#define R .5
#define NORM_Y 1/(sqrt(5))
#define NORM_XZ 2/(sqrt(5))

struct GL_Triangle {
    Vector4 coord1;
    Vector4 coord2;
    Vector4 coord3;

    Vector4 norm1;
    Vector4 norm2;
    Vector4 norm3;
};

class Shape {

public:
    //! Constructor for shape.  Pass it the initial stacks, slices (param 1 and 2)
    Shape();  //contructor

    //! Destructor for shape
    virtual ~Shape();                //destructor

    //! Builds the geometry for the shape
    virtual void createGeometry() = 0;

    //! To be overridden by subclasses, objects know how to draw themselves
    virtual void draw();

     //! To be overridden by subclasses, return array of normals
    virtual GL_Triangle* getTries();

     //! To be overridden by subclasses, return number of vertices
    virtual int getNumTries();

    //! Adds a triangle in counter-clockwise order
    virtual void addPosTriangle(int index,
                                Vector4 upper_left, Vector4 upper_right, Vector4 lower_left, Vector4 lower_right,
                                Vector4 norm1, Vector4 norm2, Vector4 norm3, Vector4 norm4);

    //! Adds a triangle in clockwise order (so I can use the same function as counter-clockwise but reverse directions)
    virtual void addNegTriangle(int index,
                                Vector4 upper_right, Vector4 upper_left, Vector4 lower_left, Vector4 lower_right,
                                Vector4 norm_ul, Vector4 norm_ur, Vector4 norm_ll, Vector4 norm_lr);

    virtual REAL checkIntersection(Vector4 eye, Vector4 ray) = 0;

    virtual Vector4 getNormal(Vector4 p_intersect) = 0;

    // for vector returned vector.x is u value, vector.y is v values
    virtual Vector2 getUV(Vector4 p_intersect_obj) = 0;


protected:
    int m_num_stacks;
    int m_num_slices;
    int m_num_faces;
    int m_num_tries;
    GL_Triangle* m_tries;

};

#endif // SHAPE_H
