#ifndef BLUR_H
#define BLUR_H

#include "Filter.h"

class Blur : public Filter {
public:
    Blur(QPoint marqee_start, QPoint marqee_end, Canvas2D* canvas);

    virtual ~Blur();

    void paint();

private:
    //do the first horizontal blur pass
    void horizontalBlur();

    //do the vertical blur pass
    void verticalBlur();

    int m_radius;
    int m_diameter;


};

#endif // BLUR_H
