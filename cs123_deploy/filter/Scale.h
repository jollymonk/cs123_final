#ifndef SCALE_H
#define SCALE_H

#include "Filter.h"

class Scale : public Filter {

public:
    Scale(QPoint marqee_start, QPoint marqee_end, Canvas2D* canvas);

    virtual ~Scale();

    void paint();

    float filter(float x, float a);

    BGRA scaleTriangleX(BGRA* orig, int x, int y, int width, float a);
    BGRA scaleTriangleY(BGRA* orig, int x, int y, int width, float a);

    void crop();

private:
    float m_scale_y, m_scale_x;
};

#endif // SCALE_H
