#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>
#include "camera.h"
#include "Emitter.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    void updateSettings();
    void updateCamera();

    bool loadImage(const QString &file);
    bool saveImage(const QString &file);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

protected slots:
    void tick();

private:
    GLUquadric *m_quadric;
    LabCamera m_camera;
    QTimer m_timer;
    float m_fps;
    float m_increment;
    Emitter** m_emitters;
    QImage *m_image;
    QString m_lastfile;
// Feel free to add your own member variables
};

#endif // GLWIDGET_H
