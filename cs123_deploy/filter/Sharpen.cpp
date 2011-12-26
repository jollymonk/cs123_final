#include "Sharpen.h"

#include <iostream>
#include <algorithm>
#include <math.h>
#include "Settings.h"

using namespace std;

Sharpen::Sharpen(QPoint marquee_start, QPoint marquee_end, Canvas2D* canvas) : Filter(marquee_start, marquee_end, canvas)
{
     // if the canvas is not null and there is an area to paint
     if (m_canvas != NULL) {

         paint();
     }
}

Sharpen::~Sharpen() {

}

void Sharpen::paint() {

    float kern [7] = {-.1, -.25, -.5, 4, -.5, -.25, -.1};
    convolve(m_pix, m_width, m_height, m_x_start, m_x_end, m_y_start, m_y_end, kern, 5);
    m_canvas->update();

}
