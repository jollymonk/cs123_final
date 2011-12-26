#include "Grayscale.h"

#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

Grayscale::Grayscale(QPoint marquee_start, QPoint marquee_end, Canvas2D* canvas) : Filter(marquee_start, marquee_end, canvas) {

    if (canvas != NULL) {
        paint();
    }
}

Grayscale::~Grayscale() {

}

void Grayscale::paint() {
    int canvas_index;

    for (int y = m_y_start; y < m_y_end; y++) {
        canvas_index = y * m_width + m_x_start;
        for (int x = m_x_start; x < m_x_end; x++) {

            float intensity = .114 * m_pix[canvas_index].b +
                              .587 * m_pix[canvas_index].g +
                              .299 * m_pix[canvas_index].r;

            //grayscale colors
            m_pix[canvas_index].b = intensity;
            m_pix[canvas_index].g = intensity;
            m_pix[canvas_index].r = intensity;

            canvas_index++;
        }
    }
    m_canvas->update();
}
