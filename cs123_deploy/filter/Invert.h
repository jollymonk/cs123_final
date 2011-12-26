#ifndef INVERT_H
#define INVERT_H

#include "Filter.h"

class Invert : public Filter{

public:
    Invert(QPoint marqee_start, QPoint marqee_end, Canvas2D* canvas);

    virtual ~Invert();

    void paint();
};

#endif // INVERT_H
