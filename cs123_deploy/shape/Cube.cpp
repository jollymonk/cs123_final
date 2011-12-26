#include "Cube.h"
#include "scenegraph/OpenGLScene.h"

#include <iostream>
#include <algorithm>
#include <math.h>
#include "RayScene.h"

using namespace std;

Cube::Cube() : Shape() {

    m_num_stacks = max(1, settings.shapeParameter1);
    createGeometry();
}

Cube::~Cube() {

}

void Cube::createGeometry() {

    int num_tries_per_face = m_num_stacks * m_num_stacks * 2;
    m_num_tries = num_tries_per_face * 6;

    double side_length = 1.0 / m_num_stacks;
    double x_start, x_const; //x will be constant in side panels
    double y_start, y_const; //y will be constant in front, back panels
    double z_start, z_const; //z will be constant in top, bottom, panels
    double face_num;

    m_tries = new GL_Triangle[m_num_tries];

    x_start = -.5;
    y_start = -.5;
    z_start = -.5;

    //****************************************************************************************************
    // MAKE TOP FACE
    //****************************************************************************************************
    face_num = 0;
    z_const = .5;

    //for every square (to be divided into triangles)
    for (int i = 0; i < m_num_stacks; i++) {
        for (int j = 0; j < m_num_stacks; j++) {

            int index = (face_num * num_tries_per_face) + (j * m_num_stacks * 2) + i * 2;
            double x_curr = x_start + side_length * i;
            double y_curr = y_start + side_length * j;m_num_stacks = max(1, m_num_stacks);

            double x_next = x_curr + side_length;
            double y_next = y_curr + side_length;

            Vector4 upper_right = Vector4(x_next, y_next, z_const, 1);
            Vector4 upper_left = Vector4(x_curr, y_next, z_const, 1);
            Vector4 lower_left = Vector4(x_curr, y_curr,  z_const, 1);
            Vector4 lower_right = Vector4(x_next, y_curr, z_const, 1);
            Vector4 normal = Vector4(0, 0, 1, 0);

            addPosTriangle(index, upper_right, upper_left, lower_left, lower_right, normal, normal, normal, normal);
        }
    }

    //****************************************************************************************************
    // MAKE BOTTOM FACE
    //****************************************************************************************************
    face_num = 1;
    z_const = -.5;

    //for every square (to be divided into triangles)
    for (int i = 0; i < m_num_stacks; i++) {
        for (int j = 0; j < m_num_stacks; j++) {

            int index = (face_num * num_tries_per_face) + (j * m_num_stacks * 2) + i * 2;
            double x_curr = x_start + side_length * i;
            double y_curr = y_start + side_length * j;

            double x_next = x_curr + side_length;
            double y_next = y_curr + side_length;

            Vector4 upper_right = Vector4(x_next, y_next, z_const, 1);
            Vector4 upper_left = Vector4(x_curr, y_next, z_const, 1);
            Vector4 lower_left = Vector4(x_curr, y_curr,  z_const, 1);
            Vector4 lower_right = Vector4(x_next, y_curr, z_const, 1);
            Vector4 normal = Vector4(0, 0, -1, 0);

            addNegTriangle(index, upper_right, upper_left, lower_left, lower_right, normal, normal, normal, normal);
        }
    }

    //****************************************************************************************************
    // MAKE BACK FACE
    //****************************************************************************************************
    face_num = 2;
    y_const = -.5;

    //for every square (to be divided into triangles)
    for (int i = 0; i < m_num_stacks; i++) {
        for (int j = 0; j < m_num_stacks; j++) {

            int index = (face_num * num_tries_per_face) + (j * m_num_stacks * 2) + i * 2;
            double x_curr = x_start + side_length * i;
            double z_curr = z_start + side_length * j;

            double x_next = x_curr + side_length;
            double z_next = z_curr + side_length;

            Vector4 upper_right = Vector4(x_next, y_const, z_next, 1);
            Vector4 upper_left = Vector4(x_curr, y_const, z_next, 1);
            Vector4 lower_left = Vector4(x_curr, y_const, z_curr, 1);
            Vector4 lower_right = Vector4(x_next, y_const, z_curr, 1);
            Vector4 normal = Vector4(0, -1, 0, 0);

            addPosTriangle(index, upper_right, upper_left, lower_left, lower_right, normal, normal, normal, normal);
        }
    }


    //****************************************************************************************************
    // MAKE FRONT FACE
    //****************************************************************************************************
    face_num = 3;
    y_const = .5;

    //for every square (to be divided into triangles)
    for (int i = 0; i < m_num_stacks; i++) {
        for (int j = 0; j < m_num_stacks; j++) {

            int index = (face_num * num_tries_per_face) + (j * m_num_stacks * 2) + i * 2;
            double x_curr = x_start + side_length * i;
            double z_curr = z_start + side_length * j;

            double x_next = x_curr + side_length;
            double z_next = z_curr + side_length;

            Vector4 upper_right = Vector4(x_next, y_const, z_next, 1);
            Vector4 upper_left = Vector4(x_curr, y_const, z_next, 1);
            Vector4 lower_left = Vector4(x_curr, y_const, z_curr, 1);
            Vector4 lower_right = Vector4(x_next, y_const, z_curr, 1);
            Vector4 normal = Vector4(0, 1, 0, 0);

            addNegTriangle(index, upper_right, upper_left, lower_left, lower_right, normal, normal, normal, normal);
        }
    }

    //****************************************************************************************************
    // MAKE RIGHT SIDE FACE
    //****************************************************************************************************
    face_num = 4;
    x_const = .5;

    //for every square (to be divided into triangles)
    for (int i = 0; i < m_num_stacks; i++) {
        for (int j = 0; j < m_num_stacks; j++) {

            int index = (face_num * num_tries_per_face) + (j * m_num_stacks * 2) + i * 2;
            double y_curr = y_start + side_length * i;
            double z_curr = z_start + side_length * j;

            double y_next = y_curr + side_length;
            double z_next = z_curr + side_length;

            Vector4 upper_right = Vector4(x_const, y_next, z_next, 1);
            Vector4 upper_left = Vector4(x_const, y_curr, z_next, 1);
            Vector4 lower_left = Vector4(x_const, y_curr,  z_curr, 1);
            Vector4 lower_right = Vector4(x_const, y_next, z_curr, 1);
            Vector4 normal = Vector4(1, 0, 0, 0);

            addPosTriangle(index, upper_right, upper_left, lower_left, lower_right, normal, normal, normal, normal);

        }
    }

    //****************************************************************************************************
    // MAKE LEFT SIDE FACE
    //****************************************************************************************************
    face_num = 5;
    x_const = -.5;

    //for every square (to be divided into triangles)
    for (int i = 0; i < m_num_stacks; i++) {
        for (int j = 0; j < m_num_stacks; j++) {

            int index = (face_num * num_tries_per_face) + (j * m_num_stacks * 2) + i * 2;
            double y_curr = y_start + side_length * i;
            double z_curr = z_start + side_length * j;

            double y_next = y_curr + side_length;
            double z_next = z_curr + side_length;

            Vector4 upper_right = Vector4(x_const, y_next, z_next, 1);
            Vector4 upper_left = Vector4(x_const, y_curr, z_next, 1);
            Vector4 lower_left = Vector4(x_const, y_curr,  z_curr, 1);
            Vector4 lower_right = Vector4(x_const, y_next, z_curr, 1);
            Vector4 normal = Vector4(-1, 0, 0, 0);

            addNegTriangle(index, upper_right, upper_left, lower_left, lower_right, normal, normal, normal, normal);

        }
    }
}

REAL Cube::checkIntersection(Vector4 p_eye, Vector4 v_dir)
{

    REAL px = p_eye.x;
    REAL py = p_eye.y;
    REAL pz = p_eye.z;
    REAL dx = v_dir.x;
    REAL dy = v_dir.y;
    REAL dz = v_dir.z;

    //the value we are ultimately solving for
    REAL t = INFINITY;
    REAL t_top, t_bottom, t_front, t_back, t_right, t_left;
    //
    REAL x_plane, y_plane, z_plane;

    //***********************************************
    // Check for intersection with FRONT and BACK
    //***********************************************

    //check front (z const)
    t_front = (.5 - pz)/dz;
    x_plane = px + t_front * dx;
    y_plane = py + t_front * dy;
    if ((x_plane - .5 > EPSILON) || (x_plane + .5 < EPSILON) ||
            (y_plane - .5 > EPSILON) || (y_plane + .5 < EPSILON) || (t_front < EPSILON)) {
        t_front = INFINITY;
    }

    t = min(t, t_front);

    //check back
    t_back = (-.5 - pz)/dz;
    x_plane = px + t_back * dx;
    y_plane = py + t_back * dy;
    if ((x_plane - .5 > EPSILON) || (x_plane + .5 < EPSILON) ||
            (y_plane - .5 > EPSILON) || (y_plane + .5 < EPSILON) || (t_back < EPSILON)) {
        t_back = INFINITY;
    }

    t = min(t, t_back);


    //****************************************************************************************************
    // Check for intersection with TOP and BOTTOM
    //****************************************************************************************************

    //check top (y const)
    t_top = (.5 - py)/dy;
    x_plane = px + t_top * dx;
    z_plane = pz + t_top * dz;
    if ((x_plane - .5 > EPSILON) || (x_plane + .5 < EPSILON) ||
            (z_plane - .5 > EPSILON) || (z_plane + .5 < EPSILON) || (t_top < EPSILON)) {
        t_top = INFINITY;
    }

    t = min(t, t_top);

    //check bottom
    t_bottom = (-.5 - py)/dy;
    x_plane = px + t_bottom * dx;
    z_plane = pz + t_bottom * dz;
    if ((x_plane - .5 > EPSILON) || (x_plane + .5 < EPSILON) ||
            (z_plane - .5 > EPSILON) || (z_plane + .5 < EPSILON) || (t_bottom < EPSILON)) {
        t_bottom = INFINITY;
    }

    t = min(t, t_bottom);

    //****************************************************************************************************
    // Check for intersection with RIGHT and LEFT
    //****************************************************************************************************

    //check right (x const)
    t_right = (.5 - px)/dx;
    y_plane = py + t_right * dy;
    z_plane = pz + t_right * dz;
    if ((y_plane - .5 > EPSILON) || (y_plane + .5 < EPSILON) ||
            (z_plane - .5 > EPSILON) || (z_plane + .5 < EPSILON) || (t_right < EPSILON)) {
        t_right = INFINITY;
    }

    t = min(t, t_right);

    //check left
    t_left = (-.5 - px)/dx;
    y_plane = py + t_left * dy;
    z_plane = pz + t_left * dz;
    if ((y_plane - .5 > EPSILON) || (y_plane + .5 < EPSILON) ||
            (z_plane - .5 > EPSILON) || (z_plane + .5 < EPSILON) || (t_left < EPSILON)) {
        t_left = INFINITY;
    }

    t = min(t, t_left);

    return t;
}

//point is in object space
Vector4 Cube::getNormal(Vector4 p_intersect)
{
    Vector4 norm;
    assert(p_intersect.w > 0.0);

    //front face
    if (fabs(p_intersect.z - .5) < EPSILON)
        norm =  Vector4(0.0, 0.0, 1.0, 0.0);

    //back face
    else if (fabs(p_intersect.z + .5) < EPSILON)
        norm = Vector4(0.0, 0.0, -1.0, 0.0);

    //top
    else if (fabs(p_intersect.y - .5) < EPSILON)
        norm = Vector4(0.0, 1.0, 0.0, 0.0);

    //bottom
    else if (fabs(p_intersect.y + .5) < EPSILON)
        norm = Vector4(0.0, -1.0, 0.0, 0.0);

    //right
    else if (fabs(p_intersect.x - .5) < EPSILON)
        norm = Vector4(1.0, 0.0, 0.0, 0.0);

    //left
    else if (fabs(p_intersect.x + .5) < EPSILON)
        norm = Vector4(-1.0, 0.0, 0.0, 0.0);

    //uh oh
    else
        norm = Vector4::zero();

    return norm;
}

Vector2 Cube::getUV(Vector4 p_intersect_obj)
{
    //uv_pair x, y represents u, v
    Vector2 uv_pair;
    REAL px = p_intersect_obj.x;
    REAL py = p_intersect_obj.y;
    REAL pz = p_intersect_obj.z;

    if (fabs(pz - 0.5) < EPSILON) {
        uv_pair.x = px + .5;
        uv_pair.y = -py + .5;
    }
    else if (fabs(pz + 0.5) < EPSILON) {
        uv_pair.x = -px + .5;
        uv_pair.y = -py + .5;
    }
    else if (fabs(py - 0.5) < EPSILON) {
        uv_pair.x = px + .5;
        uv_pair.y = pz + .5;
    }
    else if (fabs(py + 0.5) < EPSILON) {
        uv_pair.x = px + .5;
        uv_pair.y = -pz + .5;
    }
    else if (fabs(px - 0.5) < EPSILON) {
        uv_pair.x = -pz + .5;
        uv_pair.y = -py + .5;
    }
    else if (fabs(px + 0.5) < EPSILON) {
        uv_pair.x = pz + .5;
        uv_pair.y = -py + .5;
    }
    else {
        uv_pair.x = 0.0;
        uv_pair.y = 0.0;
        cout << "Something bad happened in Cube texture uvs" << endl;
    }

    return uv_pair;
}

