/*!
   @file   CS123Matrix.cpp
   @author Travis Fischer (fisch0920@gmail.com)
   @date   Fall 2008
   
   @brief
      Provides basic functionality for a templated, arbitrarily-sized matrix.
      You will need to fill this file in for the Camtrans assignment.

**/

// CS123Matrix.h can be found in /course/cs123/include/algebra
#include <CS123Matrix.h>
#include <iostream>

//@name Routines which generate specific-purpose transformation matrices
//@{---------------------------------------------------------------------
// @returns the scale matrix described by the vector
Matrix4x4 getScaleMat(const Vector4 &v_scale)
{
    assert(v_scale.w == 0.0);
    REAL x = v_scale.x;
    REAL y = v_scale.y;
    REAL z = v_scale.z;
    return Matrix4x4(x,     0,      0,      0,
                     0,     y,      0,      0,
                     0,     0,      z,      0,
                     0,     0,      0,      1.0);

}

// @returns the translation matrix described by the vector
Matrix4x4 getTransMat(const Vector4 &v_trans)
{
    REAL x = v_trans.x;
    REAL y = v_trans.y;
    REAL z = v_trans.z;
    return Matrix4x4(1.0,   0,      0,      x,
                     0,     1.0,    0,      y,
                     0,     0,      1.0,    z,
                     0,     0,      0,      1.0);

}

// @returns the rotation matrix about the x axis by the specified angle
Matrix4x4 getRotXMat (const REAL radians) {

    REAL sin_t = sin(radians);
    REAL cos_t = cos(radians);
    return Matrix4x4(1.0,    0,      0,     0,
                     0,     cos_t,  -sin_t, 0,
                     0,     sin_t,  cos_t,  0,
                     0,     0,      0,      1.0);

}

// @returns the rotation matrix about the y axis by the specified angle
Matrix4x4 getRotYMat (const REAL radians) {

    REAL sin_t = sin(radians);
    REAL cos_t = cos(radians);
    return Matrix4x4(cos_t, 0,      sin_t,  0,
                     0,     1.0,    0,      0,
                     -sin_t,0,      cos_t,  0,
                     0,     0,      0,      1.0);

}

// @returns the rotation matrix about the z axis by the specified angle
Matrix4x4 getRotZMat (const REAL radians) {

    REAL sin_t = sin(radians);
    REAL cos_t = cos(radians);
    return Matrix4x4(cos_t, -sin_t, 0,      0,
                     sin_t, cos_t,  0,      0,
                     0,     0,      1.0,    0,
                     0,     0,      0,      1.0);

}

// @returns the rotation matrix around the vector and point by the specified angle
Matrix4x4 getRotMat  (const Vector4 &p_rot, const Vector4 &v_rot, const REAL radians) {

    assert(p_rot.w > 0.0);

    assert(v_rot.w == 0.0);

    Vector4 v_rot_norm = v_rot;
    v_rot_norm.normalize();
    REAL x = v_rot_norm.x;
    REAL y = v_rot_norm.y;
    REAL z = v_rot_norm.z;
    REAL theta = atan2(z, x);
    REAL phi = -atan2(y, sqrt(x * x + z * z));

    //translate to the origin from point p, and back
    Matrix4x4 trans_to_orig = getInvTransMat(p_rot);
    Matrix4x4 trans_to_pt = getTransMat(p_rot);
    Matrix4x4 rot_y = getRotYMat(theta);               // M1
    Matrix4x4 rot_z = getRotZMat(phi);                 // M2
    Matrix4x4 rot_x = getRotXMat(radians);             // M3
    Matrix4x4 i_rot_y = getInvRotYMat(theta);          // M1 ^(-1)
    Matrix4x4 i_rot_z = getInvRotZMat(phi);            // M2 ^(-1)

    return trans_to_pt * i_rot_y * i_rot_z * rot_x * rot_z * rot_y * trans_to_orig;
}


// @returns the inverse scale matrix described by the vector
Matrix4x4 getInvScaleMat(const Vector4 &v_scale) {

    REAL x = v_scale.x;
    REAL y = v_scale.y;
    REAL z = v_scale.z;
    return Matrix4x4(1.0/x, 0,      0,      0,
                     0,     1.0/y,  0,      0,
                     0,     0,      1.0/z,  0,
                     0,     0,      0,      1.0);

}

// @returns the inverse translation matrix described by the vector
Matrix4x4 getInvTransMat(const Vector4 &v_trans) {

    REAL x = v_trans.x;
    REAL y = v_trans.y;
    REAL z = v_trans.z;
    return Matrix4x4(1.0,   0,      0,      -x,
                     0,     1.0,    0,      -y,
                     0,     0,      1.0,    -z,
                     0,     0,      0,      1.0);

}

// @returns the inverse rotation matrix about the x axis by the specified angle
Matrix4x4 getInvRotXMat (const REAL radians) {

    REAL sin_t = sin(radians);
    REAL cos_t = cos(radians);
    return Matrix4x4(1,     0,      0,      0,
                     0,     cos_t,  sin_t,  0,
                     0,     -sin_t, cos_t,  0,
                     0,     0,      0,      1.0);

}

// @returns the inverse rotation matrix about the y axis by the specified angle
Matrix4x4 getInvRotYMat (const REAL radians) {

    REAL sin_t = sin(radians);
    REAL cos_t = cos(radians);
    return Matrix4x4(cos_t, 0,      -sin_t, 0,
                     0,     1.0,    0,      0,
                     sin_t, 0,      cos_t,  0,
                     0,     0,      0,      1.0);

}

// @returns the inverse rotation matrix about the z axis by the specified angle
Matrix4x4 getInvRotZMat (const REAL radians) {

    REAL sin_t = sin(radians);
    REAL cos_t = cos(radians);
    return Matrix4x4(cos_t, sin_t,  0,      0,
                     -sin_t,cos_t,  0,      0,
                     0,     0,      1.0,    0,
                     0,     0,      0,      1.0);

}

// @returns the inverse rotation matrix around the vector and point by the specified angle
Matrix4x4 getInvRotMat  (const Vector4 &p_rot, const Vector4 &v_rot, const REAL radians) {

    assert(p_rot.w > 0.0);
    assert(v_rot.w == 0.0);

    Vector4 v_rot_norm = v_rot;
    v_rot_norm.normalize();
    REAL x = v_rot_norm.x;
    REAL y = v_rot_norm.y;
    REAL z = v_rot_norm.z;
    REAL theta = atan2(z, x);
    REAL phi = -atan2(y, sqrt(x * x + z * z));

    //translate to the origin from point p, and back
    Matrix4x4 trans_to_orig = getInvTransMat(p_rot);
    Matrix4x4 trans_to_pt = getTransMat(p_rot);
    Matrix4x4 rot_y = getRotYMat(theta);               // M1
    Matrix4x4 rot_z = getRotZMat(phi);                 // M2
    Matrix4x4 i_rot_x = getInvRotXMat(radians);         // M3 ^(-1)
    Matrix4x4 i_rot_y = getInvRotYMat(theta);          // M1 ^(-1)
    Matrix4x4 i_rot_z = getInvRotZMat(phi);            // M2 ^(-1)

    return trans_to_pt * i_rot_y * i_rot_z * i_rot_x * rot_z * rot_y * trans_to_orig;

}

// @returns a boolean of whether it contains all 0s
bool isZeroMat (const Matrix4x4 m1) {

    if (EQ(m1.a, 0) &&
            EQ(m1.b, 0) &&
            EQ(m1.c, 0) &&
            EQ(m1.d, 0) &&
            EQ(m1.e, 0) &&
            EQ(m1.f, 0) &&
            EQ(m1.g, 0) &&
            EQ(m1.h, 0) &&
            EQ(m1.i, 0) &&
            EQ(m1.j, 0) &&
            EQ(m1.k, 0) &&
            EQ(m1.l, 0) &&
            EQ(m1.m, 0) &&
            EQ(m1.n, 0) &&
            EQ(m1.o, 0) &&
            EQ(m1.p, 0)) {
        return true;
    }
    return false;
}

//returns if the 2 matrices are the same (within epsilon)
bool equals(const Matrix4x4 m1, const Matrix4x4 m2) {

    if (EQ(m1.a, m2.a) &&
            EQ(m1.b, m2.b) &&
            EQ(m1.c, m2.c) &&
            EQ(m1.d, m2.d) &&
            EQ(m1.e, m2.e) &&
            EQ(m1.f, m2.f) &&
            EQ(m1.g, m2.g) &&
            EQ(m1.h, m2.h) &&
            EQ(m1.i, m2.i) &&
            EQ(m1.j, m2.j) &&
            EQ(m1.k, m2.k) &&
            EQ(m1.l, m2.l) &&
            EQ(m1.m, m2.m) &&
            EQ(m1.n, m2.n) &&
            EQ(m1.o, m2.o) &&
            EQ(m1.p, m2.p)) {
        return true;
    }
    return false;
}

void printMatrix(Matrix4x4 m)
{
        cout << "[ " << m.a << ", " << m.b << ", " << m.c << ", " << m.d << " ]" << endl;
        cout << "[ " << m.e << ", " << m.f << ", " << m.g << ", " << m.h << " ]" << endl;
        cout << "[ " << m.i << ", " << m.j << ", " << m.k << ", " << m.l << " ]" << endl;
        cout << "[ " << m.m << ", " << m.n << ", " << m.o << ", " << m.p << " ]" << endl;
}

//@}---------------------------------------------------------------------

