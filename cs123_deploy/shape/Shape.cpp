#include "Shape.h"
#include "scenegraph/OpenGLScene.h"

#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

Shape::Shape(){

    m_num_stacks = settings.shapeParameter1;
    m_num_slices = settings.shapeParameter1;

}

Shape::~Shape() {

    delete[] m_tries;
}

GL_Triangle* Shape::getTries() {
    return m_tries;
}

int Shape::getNumTries() {
    return m_num_tries;
}

void Shape::addPosTriangle(int index,
                           Vector4 upper_right, Vector4 upper_left, Vector4 lower_left, Vector4 lower_right,
                           Vector4 norm_ur, Vector4 norm_ul, Vector4 norm_ll, Vector4 norm_lr) {

    GL_Triangle first_tri, second_tri;

    first_tri.coord1 = upper_right;
    first_tri.coord2 = upper_left;
    first_tri.coord3 = lower_left;

    first_tri.norm1 = norm_ur;
    first_tri.norm2 = norm_ul;
    first_tri.norm3 = norm_ll;

    m_tries[index] = first_tri;

    second_tri.coord1 = upper_right;
    second_tri.coord2 = lower_left;
    second_tri.coord3 = lower_right;

    second_tri.norm1 = norm_ur;
    second_tri.norm2 = norm_ll;
    second_tri.norm3 = norm_lr;

    m_tries[(index + 1)] = second_tri;
}

void Shape::addNegTriangle(int index,
                           Vector4 upper_right, Vector4 upper_left, Vector4 lower_left, Vector4 lower_right,
                           Vector4 norm_ur, Vector4 norm_ul, Vector4 norm_ll, Vector4 norm_lr) {

    GL_Triangle first_tri, second_tri;

    first_tri.coord1 = upper_right;
    first_tri.coord2 = lower_right;
    first_tri.coord3 = lower_left;

    first_tri.norm1 = norm_ur;
    first_tri.norm2 = norm_lr;
    first_tri.norm3 = norm_ll;

    m_tries[index] = first_tri;

    second_tri.coord1 = upper_right;
    second_tri.coord2 = lower_left;
    second_tri.coord3 = upper_left;

    second_tri.norm1 = norm_ur;
    second_tri.norm2 = norm_ll;
    second_tri.norm3 = norm_ul;

    m_tries[(index + 1)] = second_tri;
}

void Shape::draw() {
    glBegin(GL_TRIANGLES);

    //draw all triangles
    for (int i = 0; i < m_num_tries; i++) {

        Vector4 c1 = m_tries[i].coord1;
        Vector4 c2 = m_tries[i].coord2;
        Vector4 c3 = m_tries[i].coord3;
        Vector4 n1 = m_tries[i].norm1;
        Vector4 n2 = m_tries[i].norm2;
        Vector4 n3 = m_tries[i].norm3;

        //draw vertices and normals
        glNormal3f(n1.x, n1.y, n1.z);
        glVertex3f(c1.x, c1.y, c1.z);

        glNormal3f(n2.x, n2.y, n2.z);
        glVertex3f(c2.x, c2.y, c2.z);

        glNormal3f(n3.x, n3.y, n3.z);
        glVertex3f(c3.x, c3.y, c3.z);

    }
    glEnd();
}
