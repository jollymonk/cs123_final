#ifndef SHARPEN_H
#define SHARPEN_H

#include "Filter.h"

class Sharpen : public Filter {

public:
    Sharpen(QPoint marquee_start, QPoint marquee_end, Canvas2D* canvas);
    ~Sharpen();

    void paint();
};

#endif // SHARPEN_H
