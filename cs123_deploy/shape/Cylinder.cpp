#include "Cylinder.h"
#include "scenegraph/OpenGLScene.h"

#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

Cylinder::Cylinder() : Shape() {

    m_num_stacks = max(1, settings.shapeParameter1);
    m_num_slices = max(3, settings.shapeParameter2);

    createGeometry();

}

Cylinder::~Cylinder() {
    // m_tries deleted in superclass

}

void Cylinder::createGeometry() {

    int tries_per_slice = 2 * m_num_stacks - 1;
    int tries_per_cap = tries_per_slice * m_num_slices;
    int tries_per_side = 2 * m_num_stacks;
    m_num_tries = (2 * m_num_stacks - 1) * m_num_slices * 2 +  // number of vertices on each cap (top, bottom)
                  2 * m_num_stacks * m_num_slices;                   // number of vertices on the sides

    float theta_slice = (2 * PI)/ (float) m_num_slices;
    float radius_slice = R /m_num_stacks;
    float side_slice = (2 * R)/m_num_stacks;
    float theta_curr, theta_next;
    float radius_curr, radius_next;
    float side_curr, side_next;
    float side_start = -1 * R;

    m_tries = new GL_Triangle[m_num_tries];

    Vector4 upper_right, upper_left, lower_right, lower_left;
    Vector4 norm_ur, norm_ul, norm_lr, norm_ll;


    //****************************************************************************************************
    // MAKE TOP FACE
    //****************************************************************************************************

    float y_const = -1 * R;
    Vector4 normal = Vector4(0, -1, 0, 0);
    Vector4 center = Vector4(0, y_const, 0, 1);

    for (int i =0; i < m_num_slices; i++) {

        GL_Triangle inner;

        theta_curr = i * theta_slice;
        theta_next= (i + 1) * theta_slice;
        int index = i * tries_per_slice;

        //start with innermost triangle
        Vector4 lower_left = Vector4(radius_slice * cos(theta_curr), y_const, radius_slice * sin(theta_curr), 1);
        Vector4 lower_right = Vector4(radius_slice * cos(theta_next), y_const, radius_slice * sin(theta_next), 1);

        inner.coord1 = center;
        inner.coord2 = lower_left;
        inner.coord3 = lower_right;
        inner.norm1 = normal;
        inner.norm2 = normal;
        inner.norm3 = normal;

        m_tries[index] = inner;

        for (int j = 1; j < m_num_stacks; j++) {

            index++;
            radius_curr = j * radius_slice;
            radius_next = (j + 1) * radius_slice;

            //create the square
            upper_right = Vector4(radius_next * cos(theta_curr), y_const, radius_next * sin(theta_curr), 1);
            upper_left = Vector4(radius_next * cos(theta_next), y_const, radius_next * sin(theta_next), 1);
            lower_left = Vector4(radius_curr * cos(theta_next), y_const, radius_curr * sin(theta_next), 1);
            lower_right = Vector4(radius_curr * cos(theta_curr), y_const, radius_curr * sin(theta_curr), 1);

            addPosTriangle(index, upper_right, upper_left, lower_left, lower_right, normal, normal, normal, normal);

            index++;

        }
    }

    //****************************************************************************************************
    // MAKE BOTTOM FACE
    //****************************************************************************************************
    y_const = R;
    normal = Vector4(0, 1, 0, 0);
    center = Vector4(0, y_const, 0, 1);

    for (int i =0; i < m_num_slices; i++) {

        GL_Triangle inner;

        theta_curr = i * theta_slice;
        theta_next= (i + 1) * theta_slice;
        int index = tries_per_cap + i * tries_per_slice;

        //start with innermost triangle
        lower_left = Vector4(radius_slice * cos(theta_curr), y_const, radius_slice * sin(theta_curr), 1);
        lower_right = Vector4(radius_slice * cos(theta_next), y_const, radius_slice * sin(theta_next), 1);

        inner.coord1 = center;
        inner.coord2 = lower_right;
        inner.coord3 = lower_left;
        inner.norm1 = normal;
        inner.norm2 = normal;
        inner.norm3 = normal;

        m_tries[index] = inner;

        for (int j = 1; j < m_num_stacks; j++) {

            index++;
            radius_curr = j * radius_slice;
            radius_next = (j + 1) * radius_slice;

            //create the square
            upper_right = Vector4(radius_next * cos(theta_curr), y_const, radius_next * sin(theta_curr), 1);
            upper_left = Vector4(radius_next * cos(theta_next), y_const, radius_next * sin(theta_next), 1);
            lower_left = Vector4(radius_curr * cos(theta_next), y_const, radius_curr * sin(theta_next), 1);
            lower_right = Vector4(radius_curr * cos(theta_curr), y_const, radius_curr * sin(theta_curr), 1);

            addNegTriangle(index, upper_right, upper_left, lower_left, lower_right, normal, normal, normal, normal);

            index++;

        }
    }


    //****************************************************************************************************
    // MAKE SIDES
    //****************************************************************************************************
    for (int i =0; i < m_num_slices; i++) {

        theta_curr = i * theta_slice;
        theta_next= (i + 1) * theta_slice;
        int index = tries_per_cap * 2 + i * tries_per_side;

        for (int j = 0; j < m_num_stacks; j++) {

            side_curr = side_start + j * side_slice;
            side_next = side_start + (j + 1) * side_slice;

            //create the square

            Vector4 norm_theta_curr = Vector4(cos(theta_curr), 0, sin(theta_curr), 0);
            Vector4 norm_theta_next = Vector4(cos(theta_next), 0, sin(theta_next), 0);
            norm_theta_curr = norm_theta_curr.getNormalized();
            norm_theta_next = norm_theta_next.getNormalized();

            upper_right = Vector4(R * cos(theta_next), side_next, R * sin(theta_next), 1);
            upper_left = Vector4(R * cos(theta_curr), side_next, R * sin(theta_curr), 1);
            lower_left = Vector4(R * cos(theta_curr), side_curr, R * sin(theta_curr), 1);
            lower_right = Vector4(R * cos(theta_next), side_curr, R * sin(theta_next), 1);

            addNegTriangle(index, upper_right, upper_left, lower_left, lower_right,
                           norm_theta_next, norm_theta_curr, norm_theta_curr, norm_theta_next);

            index = index + 2;

        }
    }
}

REAL Cylinder::checkIntersection(Vector4 p_eye, Vector4 v_dir)
{
    REAL px = p_eye.x;
    REAL py = p_eye.y;
    REAL pz = p_eye.z;
    REAL dx = v_dir.x;
    REAL dy = v_dir.y;
    REAL dz = v_dir.z;

    REAL t = INFINITY;
    REAL t_sides, t_top_cap, t_bottom_cap;

    //***********************************************
    // Check for intersection with sides
    //***********************************************

    //The a, b, and c values for my implicit equation
    REAL a, b, c;
    a = dx * dx + dz * dz;
    b = 2.0 * (px * dx + pz * dz);
    c = px * px + pz * pz - 0.25;

    REAL disc = b * b - 4.0 * a * c;

    if (disc < 0) {
        return INFINITY;
    }

    disc = sqrt(disc);

    REAL t1, t2;
    t1 = (-1.0 * b + disc) / (2.0 * a);

    REAL y;
    y = py + t1 * dy;
    //doesn't intersect
    if ((.5 + y < 0.0) || (.5 - y < 0.0) || (t1 < EPSILON))
        t1 = INFINITY;

    t2 = (-1.0 * b - disc) / (2.0 * a);

    y = py + t2 * dy;
    //doesn't intersect
    if ((.5 + y < 0.0) || (.5 - y < 0.0) || (t2 < EPSILON))
        t2 = INFINITY;

    t_sides = min(t1, t2);

    //***********************************************
    // Check for intersection with lower cap
    //***********************************************

    REAL x, z;
    t_bottom_cap = (-.5 - py)/dy;
    x = px + t_bottom_cap * dx;
    z = pz + t_bottom_cap * dz;
    if ((x * x + z * z -.25 > 0.0) || (t_bottom_cap < EPSILON)) {
        t_bottom_cap = INFINITY;
    }

    t = min(t_sides, t_bottom_cap);

    //***********************************************
    // Check for intersection with upper cap
    //***********************************************

    t_top_cap = (.5 - py)/dy;
    x = px + t_top_cap * dx;
    z = pz + t_top_cap * dz;
    if ((x * x + z * z -.25 > 0.0) || (t_top_cap < EPSILON)) {
        t_top_cap = INFINITY;
    }

    t = min(t, t_top_cap);

    return t;
}

//point is in object space
Vector4 Cylinder::getNormal(Vector4 p_intersect)
{
    assert(p_intersect.w > 0.0);
    Vector4 norm;

    //if the point is on the bottom cap
    if (fabs(p_intersect.y + .5) < EPSILON)
        norm = Vector4(0.0, -1.0, 0.0, 0.0);

//    //if the point is on the top cap
    else if(fabs(p_intersect.y - .5) < EPSILON)
        norm = Vector4(0.0, 1.0, 0.0, 0.0);

    //if the point is on the sides
    else
    {
        norm = Vector4(p_intersect.x, 0.0, p_intersect.z, 0.0);
        norm = norm/.5;
        norm = norm.getNormalized();
    }

    return norm;
}

Vector2 Cylinder::getUV(Vector4 p_intersect_obj)
{
    //uv_pair x, y represents u, v
    Vector2 uv_pair;
    REAL px = p_intersect_obj.x;
    REAL py = p_intersect_obj.y;
    REAL pz = p_intersect_obj.z;

    //check the top cap
    if (fabs(py - .5) < EPSILON) {
        uv_pair.x = px + .5;
        uv_pair.y = pz + .5;
    }
    //check the bottom cap
    else if (fabs(py + .5) < EPSILON) {
        uv_pair.x = px + .5;
        uv_pair.y = -pz + .5;
    }
    //check the sides
    else {
        uv_pair.y = -py + .5;

        REAL theta = atan2(pz, px);
        if (theta < 0.0)
            uv_pair.x = -theta / (2.0 * M_PI);
        else
            uv_pair.x = -theta / (2.0 * M_PI) + 1.0;
    }

    return uv_pair;
}
