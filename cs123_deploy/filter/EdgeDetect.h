#ifndef EDGEDETECT_H
#define EDGEDETECT_H

#include "Filter.h"

class EdgeDetect : public Filter {

public:
    EdgeDetect(QPoint marqee_start, QPoint marqee_end, Canvas2D* canvas);

    virtual ~EdgeDetect();

    void paint();
};

#endif // EDGEDETECT_H
