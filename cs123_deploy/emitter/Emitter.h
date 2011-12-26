#ifndef EMITTER_H
#define EMITTER_H

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "CS123Common.h"
#include "Settings.h"
#include "Canvas2D.h"
#include <QtOpenGL>
#include <QGLWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QGLShader>
#include <QGLShaderProgram>
#include <QFile>
#include <iostream>

using namespace std;

class Emitter
{
    struct Droplet
    {
            const int id;
            Vector3 velocity;
            Vector3 init_pos;
            Vector3 pos;
    };

public:
    Emitter();
    ~Emitter();
    void drawDrops();
    void resetDrops();

protected:
    std::list<Droplet> *m_active_droplets;
    GLUquadric *m_quadric;
    QTimer *m_timer;
};
#endif // EMITTER_H
