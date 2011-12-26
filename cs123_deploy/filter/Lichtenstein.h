#ifndef LICHTENSTEIN_H
#define LICHTENSTEIN_H

#include "Filter.h"

class Lichtenstein : public Filter {

public:
    Lichtenstein(QPoint marquee_start, QPoint marquee_end, Canvas2D* canvas);
    ~Lichtenstein();

    void paint();

private:
    int m_dot_radius;
};

#endif // LICHTENSTEIN_H
