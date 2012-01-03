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
    bool loadImage(QImage *image, const QString &file);
    bool saveImage(const QString &file);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void setPattern();
    void paintScene();
    void getTimeDigit(QImage *image, int digit);

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
    QImage *m_concrete;
    //images of nums for displaying times
    QImage *m_img_zero;
    QImage *m_img_one;
    QImage *m_img_two;
    QImage *m_img_three;
    QImage *m_img_four;
    QImage *m_img_five;
    QImage *m_img_six;
    QImage *m_img_seven;
    QImage *m_img_eight;
    QImage *m_img_nine;
    QImage *m_img_colon;
    QImage *m_checkered;
    QImage *m_fan;
    QImage *m_zebra;
    QImage *m_hour_ls; //least significant hour digit
    QImage *m_hour_ms; //most significant hour digit
    QImage *m_min_ls;
    QImage *m_min_ms;
    int m_img_height;
    int m_img_width;
    int m_img_scaled_height;
    int m_num_width;
    int m_hour;
    int m_min;
    int m_curr_hour;
    int m_curr_min;
    double m_img_scale;
    int m_curr_height;
    int m_curr_pattern;
    QString m_lastfile;
// Feel free to add your own member variables
};

#endif // GLWIDGET_H
