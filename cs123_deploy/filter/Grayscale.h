#ifndef GRAYSCALE_H
#define GRAYSCALE_H

#include "Filter.h"

class Grayscale : public Filter {

public:
    Grayscale(QPoint marqee_start, QPoint marqee_end, Canvas2D* canvas);

    virtual ~Grayscale();

    void paint();

};

#endif // GRAYSCALE_H
