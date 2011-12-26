/*!
   @file   CamtransCamera.cpp
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief  This is the perspective camera class you will need to fill in for the
   Camtrans assignment.  See the assignment handout for more details.

   For extra credit, you can also create an Orthographic camera. Create another
   class if you want to do that.

*/

#include "CamtransCamera.h"
#include <Settings.h>
#include <qgl.h>
#include "assert.h"
#include <stdio.h>

CamtransCamera::CamtransCamera()
{
    //initial setup
    m_near = 1.0;
    m_far = 30.0;
    setAspectRatio(1.0);
    setHeightAngle(60.0);
    m_position = Vector4(2.0, 2.0, 2.0, 1.0);
    m_look = Vector4(0.0, 0.0, -1.0, 0.0);
    m_up = Vector4(0.0, 1.0, 0.0, 0.0);

    m_w = -m_look/(m_look.getMagnitude());
    m_v = m_up - (m_up.dot(m_w) * m_w);
    m_v = m_v/ (m_v.getMagnitude());
    m_u = m_v.cross(m_w);
    m_u.w = 0.0, m_v.w = 0.0, m_w.w = 0.0;
}

void CamtransCamera::setAspectRatio(float aspect)
{
    m_aspect = min(aspect, 180.f);
    setHeightAngle(toDegrees(m_h_radians));

}

Matrix4x4 CamtransCamera::getProjectionMatrix() const
{
    return getM1() * getM2();
}

Matrix4x4 CamtransCamera::getModelviewMatrix() const
{
    return getM3() * getM4();
}

Matrix4x4 CamtransCamera::getFilmtoWorldMatrix() const
{
    return getM4Inv() * getM3Inv() * getM2Inv();
}

Vector4 CamtransCamera::getPosition() const
{
    return m_position;
}

Vector4 CamtransCamera::getLook() const
{
    return m_look;
}

Vector4 CamtransCamera::getUp() const
{
    return m_up;
}

float CamtransCamera::getAspectRatio() const
{
    return m_aspect;
}

float CamtransCamera::getHeightAngle() const
{
    return toDegrees(m_h_radians);
}

void CamtransCamera::orientLook(const Vector4 &eye, const Vector4 &look, const Vector4 &up)
{
   m_position = eye;
   m_position.w = 1.0;

   m_w = -look/(look.getMagnitude());
   m_v = up - (up.dot(m_w) * m_w);
   m_v = m_v/(m_v.getMagnitude());
   m_u = m_v.cross(m_w);
   m_u.w = 0.0, m_v.w = 0.0, m_w.w = 0.0;

   m_look = -1.0 * m_w;
   m_up = m_v;
}

void CamtransCamera::setHeightAngle(float degrees)
{
    // takes in the height angle

    m_h_radians = toRadians(degrees);
    m_tan_h2 = tan(m_h_radians/2.0);
    REAL height = 2.0 * m_far * m_tan_h2;
    m_tan_w2 = height * m_aspect/(2.0 * m_far);
}

void CamtransCamera::translate(const Vector4 &v_trans)
{
    assert(v_trans.w == 0.0);
    m_position += v_trans;
}

void CamtransCamera::rotateU(float degrees)
{
   //move axes
    REAL radians = toRadians(degrees);
    Matrix4x4 mat_rot = getRotMat(m_position, -1.0 * m_u, radians);
    m_v = mat_rot * m_v;
    m_w = mat_rot * m_w;

    m_up = m_v;
    m_look = -1.0 * m_w;
}

void CamtransCamera::rotateV(float degrees)
{
    //move axes
     REAL radians = toRadians(degrees);
     Matrix4x4 mat_rot = getRotMat(m_position, m_v, radians);
     m_u = mat_rot * m_u;
     m_w = mat_rot * m_w;

     m_up = m_v;
     m_look = -1.0 * m_w;
}

void CamtransCamera::rotateN(float degrees)
{
    //move axes
     REAL radians = toRadians(degrees);
     Matrix4x4 mat_rot = getRotMat(m_position, m_w, radians);
     m_u = mat_rot * m_u;
     m_v = mat_rot * m_v;

     m_up = m_v;
     m_look = -1.0 * m_w;
}

void CamtransCamera::setClip(float nearPlane, float farPlane)
{
    m_near = nearPlane;
    m_far = farPlane;

}

REAL CamtransCamera::toDegrees(REAL radians) const
{
    return radians * (180.0/M_PI);
}

REAL CamtransCamera::toRadians(REAL degrees) const
{
    return degrees * (M_PI/180.0);
}

//unhinging matrix, does perspective transformation
Matrix4x4 CamtransCamera::getM1() const
{
    REAL c = (-1.0) * m_near/m_far;
    REAL c1 = -1.0/(c + 1.0);
    REAL c2 = c/(c + 1.0);

    Matrix4x4 m1(1.0,   0,      0,      0,
                0,      1.0,    0,      0,
                0,      0,      c1,     c2,
                0,      0,      -1.0,   0);

    return m1;
}

Matrix4x4 CamtransCamera::getM1Inv() const
{
    return getM1().getInverse();
}

//shrinks back clipping plane to z=-1, scales corners of back clipping
Matrix4x4 CamtransCamera::getM2() const
{    

    REAL h = m_tan_h2 * m_far;
    REAL w = m_tan_w2 * m_far;

    Matrix4x4 m2(1.0/w,  0,     0,      0,
                0,      1.0/h,  0,      0,
                0,      0,      1.0/m_far,0,
                0,      0,      0,      1.0);
    return m2;
}

Matrix4x4 CamtransCamera::getM2Inv() const
{
    return getM2().getInverse();
}

//rotates the view to be aligned with world coordinates
Matrix4x4 CamtransCamera::getM3() const
{
    Matrix4x4 m3(m_u.x, m_u.y, m_u.z, 0,
                m_v.x, m_v.y, m_v.z, 0,
                m_w.x, m_w.y, m_w.z, 0,
                0,     0,     0,     1);
    return m3;
}

Matrix4x4 CamtransCamera::getM3Inv() const
{
    Matrix4x4 m3_inv(m_u.x, m_v.x, m_w.x, 0,
                m_u.y, m_v.y, m_w.y, 0,
                m_u.z, m_v.z, m_w.z, 0,
                0,     0,     0,     1);
    return m3_inv;
}

//translates to the origin
Matrix4x4 CamtransCamera::getM4() const
{
    Matrix4x4 m4 = getInvTransMat(m_position);
    return m4;
}

Matrix4x4 CamtransCamera::getM4Inv() const
{
    Matrix4x4 m4_inv = getTransMat(m_position);
    return m4_inv;
}

