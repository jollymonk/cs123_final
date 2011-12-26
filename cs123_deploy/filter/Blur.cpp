#include "Blur.h"

#include <iostream>
#include <algorithm>
#include <math.h>
#include "Settings.h"
#include "Canvas2D.h"

using namespace std;

Blur::Blur(QPoint marquee_start, QPoint marquee_end, Canvas2D* canvas) : Filter(marquee_start, marquee_end, canvas) {

    m_radius = settings.blurRadius;
    m_diameter = m_radius * 2 + 1;

    // if the canvas is not null and there is an area to paint
    if (canvas != NULL) {
        paint();
    }
}

Blur::~Blur() {

}

void Blur::paint() {

    //do a horizontal motion blur
    horizontalBlur();
    verticalBlur();

    m_canvas->update();
}

void Blur::horizontalBlur() {

    int index, init_index;
    int b_accum, g_accum, r_accum;
    int last, next;

    for (int y = m_y_start; y <= m_y_end; y++) {

        index = y * m_width + m_x_start;

        //initialize first accumulation
        b_accum = (m_radius + 1) * m_pix[index].b;
        g_accum = (m_radius + 1) * m_pix[index].g;
        r_accum = (m_radius + 1) * m_pix[index].r;

        for (int k = 0; k < m_radius; k++) {

            init_index = y * m_width + min(m_x_end, m_x_start + k);;

            b_accum += m_pix[init_index].b;
            g_accum += m_pix[init_index].g;
            r_accum += m_pix[init_index].r;
        }

        m_pix[index].b = colorRange(b_accum/m_diameter);
        m_pix[index].g = colorRange(g_accum/m_diameter);
        m_pix[index].r = colorRange(r_accum/m_diameter);

        for (int x = m_x_start + 1; x <= m_x_end; x++) {

            last = y * m_width + max(m_x_start, (x - m_radius - 1));
            next = y * m_width + min(m_x_end, x + m_radius);

            //remove the accumulation of the last m_pixel in the kernel
             b_accum -= m_pix[last].b;
             g_accum -= m_pix[last].g;
             r_accum -= m_pix[last].r;

            //add the accumulation of the max value of the kernel
             b_accum += m_pix[next].b;
             g_accum += m_pix[next].g;
             r_accum += m_pix[next].r;

             //set the current pixel value
             m_pix[index].b = colorRange(b_accum/m_diameter);
             m_pix[index].g = colorRange(g_accum/m_diameter);
             m_pix[index].r = colorRange(r_accum/m_diameter);
            index++;
        }
    }
}

void Blur::verticalBlur(){

    int index, init_index;
    int b_accum, g_accum, r_accum;
    int last, next;

    for (int x = m_x_start; x <= m_x_end; x++) {

        index = m_y_start * m_width + x;

        //initialize first accumulation
        b_accum = (m_radius + 1) * m_pix[index].b;
        g_accum = (m_radius + 1) * m_pix[index].g;
        r_accum = (m_radius + 1) * m_pix[index].r;

        for (int k = 0; k <= m_radius; k++) {
            init_index = min(m_y_end, m_y_start + k) * m_width + x;

            b_accum += m_pix[init_index].b;
            g_accum += m_pix[init_index].g;
            r_accum += m_pix[init_index].r;
        }

        m_pix[index].b = colorRange(b_accum/m_diameter);
        m_pix[index].g = colorRange(g_accum/m_diameter);
        m_pix[index].r = colorRange(r_accum/m_diameter);

        for (int y = m_y_start + 1; y <= m_y_end; y++) {

            index = y * m_width + x;

            last = max(m_y_start, (y - m_radius - 1)) * m_width + x;
            next = min(m_y_end, y + m_radius) * m_width + x;

            //remove the accumulation of the last m_pixel in the kernel
            b_accum -= m_pix[last].b;
            g_accum -= m_pix[last].g;
            r_accum -= m_pix[last].r;

            //add the accumulation of the max value of the kernel
            b_accum += m_pix[next].b;
            g_accum += m_pix[next].g;
            r_accum += m_pix[next].r;

            m_pix[index].b = colorRange(b_accum/m_diameter);
            m_pix[index].g = colorRange(g_accum/m_diameter);
            m_pix[index].r = colorRange(r_accum/m_diameter);
        }
    }
}
