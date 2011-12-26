#include "Sphere.h"
#include "scenegraph/OpenGLScene.h"

#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

Sphere::Sphere() : Shape() {

    m_num_stacks = max(2, settings.shapeParameter1);
    m_num_slices = max(3, settings.shapeParameter2);

    createGeometry();
}

Sphere::~Sphere() {
    // norm and vert arrays deleted in superclass

}

void Sphere::createGeometry() {

   int tries_per_slice = 2 * m_num_stacks - 2;
    m_num_tries = tries_per_slice * m_num_slices;

    m_tries = new GL_Triangle[m_num_tries];

    float phi_slice = PI / m_num_stacks;
    Vector4 norm_top = Vector4(0, 1, 0, 0);
    Vector4 norm_bottom = Vector4(0, -1, 0, 0);
    float theta_slice = (2 * PI) / m_num_slices;

    float theta_curr, theta_next;
    float phi_curr, phi_next;

    Vector4 coord_l = Vector4(-.5, 0, 0, 1);
    Vector4 coord_r = Vector4(0, 0, .5, 1);
    Vector4 coord_t = Vector4(.5, 0, .5, 1);

    GL_Triangle test;
    test.coord1 = coord_t;
    test.coord2 = coord_l;
    test.coord3 = coord_r;

    test.norm1 = norm_top;
    test.norm2 = norm_top;
    test.norm3 = norm_top;

    Vector4 upper_right, upper_left, lower_left, lower_right;
    Vector4 norm_ur, norm_ul, norm_ll, norm_lr;

    // calculate the tries for the slices
    for (int i = 0; i < m_num_slices; i++) {

        GL_Triangle top_tri, bottom_tri;

        theta_curr = i * theta_slice;
        theta_next= (i + 1) * theta_slice;
        int index =  tries_per_slice * i;

        //make top triangle of slice
        Vector4 top = Vector4(0, R, 0, 1);

        lower_left = sphereCoord(theta_curr, phi_slice);
        lower_right = sphereCoord(theta_next, phi_slice);

        norm_ll = sphereNorm(theta_curr, phi_slice);
        norm_lr = sphereNorm(theta_next, phi_slice);

        top_tri.coord1 = top;
        top_tri.coord2 = lower_right;
        top_tri.coord3 = lower_left;

        top_tri.norm1 = norm_top;
        top_tri.norm2 = norm_lr;
        top_tri.norm3 = norm_ll;

        m_tries[index] = top_tri;
        index++;

        //make bottom triangle of slice
        Vector4 bottom = Vector4(0, (-1 * R), 0, 1);

        float phi_bottom = PI - phi_slice;

        lower_left = sphereCoord(theta_curr, phi_bottom);
        lower_right = sphereCoord(theta_next, phi_bottom);

        norm_ll = sphereNorm(theta_curr, phi_bottom);
        norm_lr = sphereNorm(theta_next, phi_bottom);

        bottom_tri.coord1 = bottom;
        bottom_tri.coord2 = lower_left;
        bottom_tri.coord3 = lower_right;

        bottom_tri.norm1 = norm_bottom;
        bottom_tri.norm2 = norm_ll;
        bottom_tri.norm3 = norm_lr;

        m_tries[index] = bottom_tri;

        //make main side slices
        for (int j = 1; j < (m_num_stacks - 1); j++) {

            index = index++;

            phi_curr = j * phi_slice;
            phi_next = (j + 1) * phi_slice;

            //calculate vector coordinates
            Vector4 upper_left, upper_right, lower_right, lower_left;

            upper_right = sphereCoord(theta_curr, phi_curr);
            upper_left = sphereCoord(theta_next, phi_curr);
            lower_left = sphereCoord(theta_next, phi_next);
            lower_right = sphereCoord(theta_curr, phi_next);

            //calculate normal coordinates
            Vector4 norm_ul, norm_ur, norm_ll, norm_lr;

            norm_ur = sphereNorm(theta_curr, phi_curr);
            norm_ul = sphereNorm(theta_next, phi_curr);
            norm_ll = sphereNorm(theta_next, phi_next);
            norm_lr = sphereNorm(theta_curr, phi_next);

            addPosTriangle(index, upper_right, upper_left, lower_left, lower_right, norm_ur, norm_ul, norm_ll, norm_lr);

            index++;
        }
    }
}

Vector4 Sphere::sphereCoord(double theta, double phi) {
    return Vector4(R * sin(phi) * cos(theta),
                      R * cos(phi),
                      R * sin(phi) * sin(theta), 1);
}

Vector4 Sphere::sphereNorm(double theta, double phi) {
    Vector4 norm = Vector4(R * sin(phi) * cos(theta),
                      R * cos(phi),
                      R * sin(phi) * sin(theta), 0.0);
    //ignores w
    norm = norm.getNormalized();

   return norm;
}

REAL Sphere::checkIntersection(Vector4 p_eye, Vector4 v_dir)
{

    REAL px = p_eye.x;
    REAL py = p_eye.y;
    REAL pz = p_eye.z;
    REAL dx = v_dir.x;
    REAL dy = v_dir.y;
    REAL dz = v_dir.z;

    //solve quadratic equation
    REAL t = INFINITY;
    REAL t1, t2;
    REAL a, b, c;
    a = dx * dx + dy * dy + dz * dz;
    b = 2.0 * (px * dx + py * dy + pz * dz);
    c = px * px + py * py + pz * pz - 0.25;

    REAL disc = b * b - 4.0 * a * c;

    if (disc < 0) {
        return INFINITY;
    }

    disc = sqrt(disc);
    t1 = (-1.0 * b + disc) / (2.0 * a);
    t2 = (-1.0 * b - disc) / (2.0 * a);

    if (t1 < EPSILON)
            t1 = INFINITY;
    if (t2 < EPSILON)
            t2 = INFINITY;

    t = min(t1, t2);
    return t;
}

//point passed in is in object space
Vector4 Sphere::getNormal(Vector4 p_intersect)
{
    assert(p_intersect.w > 0.0);

    Vector4 norm = Vector4(p_intersect.x, p_intersect.y, p_intersect.z, 0.0);
    norm = norm/.5;
    norm = norm.getNormalized();
    return norm;
}

Vector2 Sphere::getUV(Vector4 p_intersect_obj)
{
    //uv_pair x, y represents u, v
    Vector2 uv_pair;
    REAL px = p_intersect_obj.x;
    REAL py = p_intersect_obj.y;
    REAL pz = p_intersect_obj.z;

    REAL phi = asin(py/.5);
    uv_pair.y = -phi/ M_PI + .5;

    if (uv_pair.y == 0.0 || uv_pair.y == 1.0)
        uv_pair.x = .5;
    else
    {
        REAL theta = atan2(pz, px);
        if (theta < 0)
            uv_pair.x = -theta / (2.0 * M_PI);
        else
            uv_pair.x = -theta / (2.0 * M_PI) + 1.0;
    }

    return uv_pair;
}
