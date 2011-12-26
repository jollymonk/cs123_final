#include "Invert.h"

#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

Invert::Invert(QPoint marquee_start, QPoint marquee_end, Canvas2D* canvas) : Filter(marquee_start, marquee_end, canvas) {

    if (canvas != NULL) {
        paint();
    }
}

Invert::~Invert() {

}

void Invert::paint() {
    int index;

    for (int y = m_y_start; y <= m_y_end; y++) {
        index = y * m_width + m_x_start;
        for (int x = m_x_start; x <= m_x_end; x++) {

            //invert colors
            m_pix[index] = BGRA(
                        255 - m_pix[index].b,
                        255 - m_pix[index].g,
                        255 - m_pix[index].r,
                        255);

            index++;
        }
    }

    m_canvas->update();
}
