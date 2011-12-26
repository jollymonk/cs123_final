#include "Cone.h"
#include "scenegraph/OpenGLScene.h"

#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

Cone::Cone() : Shape(){

    m_num_stacks = max(1, settings.shapeParameter1);
    m_num_slices = max(3, settings.shapeParameter2);

    createGeometry();
}

Cone::~Cone() {

}

void Cone::createGeometry() {

    // calculate number of tries
    int tries_per_slice = 2 * m_num_stacks - 1;
    int tries_per_cap = tries_per_slice * m_num_slices;
    m_num_tries = tries_per_slice * m_num_slices * 2;

    // calculate the size of each slice of angles, radii, and side_length
    float theta_slice = (2 * PI)/ (float) m_num_slices;
    float radius_slice = R /m_num_stacks;
    float side_slice = (2 * R)/m_num_stacks;
    float theta_curr, theta_next, theta_mid;
    float radius_curr, radius_next;
    float side_curr, side_next;
    float side_start = R;

    Vector4 upper_right, upper_left, lower_left, lower_right;
    Vector4 norm_ur, norm_ul, norm_ll, norm_lr;

    m_tries = new GL_Triangle[m_num_tries];

    //****************************************************************************************************
    // MAKE BOTTOM FACE
    //****************************************************************************************************
    float y_const = -.5;
    Vector4 normal = Vector4(0, -1, 0, 0);
    Vector4 center = Vector4(0, y_const, 0, 1);

    for (int i =0; i < m_num_slices; i++) {

        GL_Triangle inner;

        theta_curr = i * theta_slice;
        theta_next= (i + 1) * theta_slice;
        int index =  i * tries_per_slice;

        //start with innermost triangle
        lower_left = Vector4(radius_slice * cos(theta_curr), y_const, radius_slice * sin(theta_curr), 1);
        lower_right = Vector4(radius_slice * cos(theta_next), y_const, radius_slice * sin(theta_next), 1);

        inner.coord1 = center;
        inner.coord2 = lower_left;
        inner.coord3 = lower_right;
        inner.norm1 = normal;
        inner.norm2 = normal;
        inner.norm3 = normal;

        m_tries[index] = inner;

        //calculate the vertices for each square
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
    // MAKE SIDES
    //****************************************************************************************************
    normal = Vector4(0, 1, 0, 0);
    Vector4 top = Vector4(0, R, 0, 1);
    for (int i =0; i < m_num_slices; i++) {

        GL_Triangle top_tri;

        theta_curr = i * theta_slice;
        theta_next= (i + 1) * theta_slice;
        theta_mid = (theta_curr + theta_next) / 2;
        int index =  tries_per_cap + i * tries_per_slice;

        side_curr = side_start - side_slice;
        radius_curr = radius_slice;

        norm_ur = coneNorm(theta_curr);
        norm_ul = coneNorm(theta_next);
        norm_ll = coneNorm(theta_next);
        norm_lr = coneNorm(theta_curr);
        Vector4 norm_top = coneNorm(theta_mid);

        //make with top triangle of each slice
        lower_left = Vector4(radius_slice * cos(theta_curr), side_curr, radius_slice * sin(theta_curr), 1);
        lower_right = Vector4(radius_slice * cos(theta_next), side_curr, radius_slice * sin(theta_next), 1);

        top_tri.coord1 = top;
        top_tri.coord2 = lower_right;
        top_tri.coord3 = lower_left;
        top_tri.norm1 = norm_top;
        top_tri.norm2 = norm_ul;
        top_tri.norm3 = norm_ur;

        m_tries[index] = top_tri;

        for (int j = 1; j < m_num_stacks; j++) {

            index++;
            side_curr = side_start - j * side_slice;
            side_next = side_start - (j + 1) * side_slice;

            radius_curr = j * radius_slice;
            radius_next = (j + 1) * radius_slice;

            //create the square
            upper_right = Vector4(radius_next * cos(theta_curr), side_next, radius_next * sin(theta_curr), 1);
            upper_left = Vector4(radius_next * cos(theta_next), side_next, radius_next * sin(theta_next), 1);
            lower_left = Vector4(radius_curr * cos(theta_next), side_curr, radius_curr * sin(theta_next), 1);
            lower_right = Vector4(radius_curr * cos(theta_curr), side_curr, radius_curr * sin(theta_curr), 1);

            addNegTriangle(index, upper_right, upper_left, lower_left, lower_right, norm_ur, norm_ul, norm_ll, norm_lr);

            index++;
        }
    }
}

Vector4 Cone::coneNorm(double theta) {
    //NORM_XY and NORM_Y are the constant values 2/sqrt(5) and 1/sqrt(5) defined in Shape.h

    float length = sqrt(1 - NORM_Y * NORM_Y);
    float x = (R * cos(theta))/length;
    float z = (R * sin(theta))/length;
    Vector4 norm = Vector4(x, NORM_Y, z, 0.0);
    norm = norm.getNormalized();

   return norm;
}

REAL Cone::checkIntersection(Vector4 p_eye, Vector4 v_dir)
{

    REAL px = p_eye.x;
    REAL py = p_eye.y;
    REAL pz = p_eye.z;
    REAL dx = v_dir.x;
    REAL dy = v_dir.y;
    REAL dz = v_dir.z;

    //the value we are ultimately solving for
    REAL t = INFINITY;

    //****************************************************************************************************
    // Check for intersection with sides
    //****************************************************************************************************

    // a, b, and c of the quadratic etqn
    REAL a, b, c;
    a = dx * dx + dz * dz - .25 * dy * dy;
    b = 2.0 * (px * dx + pz * dz - .25 * py * dy + .125 * dy);
    c = px * px + pz * pz - .25 * py * py + .25 * py - .0625;

    //discriminant
    REAL disc = b * b - 4.0 * a * c;

    //return infinite if there is no intersection
    if (disc < 0)
        return INFINITY;

    disc = sqrt(disc);

    //solve quadratic equation
    REAL t1, t2, t_sides, y;

    t1 = (-1.0 * b + disc) / (2.0 * a);
    y = py + t1 * dy;

    if ((.5 + y < 0.0) || (.5 - y < 0.0) || (t1 < EPSILON))
        t1 = INFINITY;

    t2 = (-1.0 * b - disc) / (2.0 * a);
    y = py + t2 * dy;

    if ((.5 + y < 0.0) || (.5 - y < 0.0) || (t2 < EPSILON))
        t2 = INFINITY;

    t_sides = min(t1, t2);


    //****************************************************************************************************
    // Check for intersection with cap
    //****************************************************************************************************

    REAL t_cap, cap_x, cap_z;

    t_cap = (-.5 - py)/dy;
    cap_x = px + t_cap * dx;
    cap_z = pz + t_cap * dz;

    if ((cap_x * cap_x + cap_z * cap_z - .25) > 0 || t_cap < EPSILON)
        t_cap = INFINITY;

    t = min(t_sides, t_cap);

    return t;
}

Vector4 Cone::getNormal(Vector4 p_intersect_obj)
{
    assert(p_intersect_obj.w > 0.0);
    Vector4 norm;

    //check if the point is on the cap
    if (fabs(p_intersect_obj.y + .5) < EPSILON) {
        norm = Vector4(0, -1, 0, 0);
    }

    //point is on the side, normal is the slope
    else {
        norm = Vector4(2.0 * p_intersect_obj.x,
                (.5 * (.5 - p_intersect_obj.y)),
                2.0 * p_intersect_obj.z, 0.0);
        norm = norm.getNormalized();
    }

    return norm;
}

Vector2 Cone::getUV(Vector4 p_intersect_obj)
{
    //uv_pair x, y represents u, v
    Vector2 uv_pair;
    REAL px = p_intersect_obj.x;
    REAL py = p_intersect_obj.y;
    REAL pz = p_intersect_obj.z;

    //check bottom cap
    if(fabs(py + .5) < EPSILON) {
        uv_pair.x = px + .5;
        uv_pair.y = -pz + .5;
    }
    //the main body of the cone
    else {
        uv_pair.y = -py + .5;
        REAL theta = atan2(pz, px);
        if (theta < 0.0)
            uv_pair.x = -theta / (2.0 * M_PI);
        else
            uv_pair.x = -theta / (2.0 * M_PI) + 1.0;
    }

//    uv_pair.x = px + .5;
//    uv_pair.y = -px + .5;

    return uv_pair;
}

