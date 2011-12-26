#include "Filter.h"
#include "scenegraph/OpenGLScene.h"

#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

Filter::Filter(QPoint marquee_start, QPoint marquee_end, Canvas2D* canvas) {

    m_canvas = canvas;

    if (marquee_start != marquee_end) {
        //if there is a marquee selection, then choose the selection
        m_x_start = min(marquee_start.x(), marquee_end.x());
        m_y_start = min(marquee_start.y(), marquee_end.y());
        m_x_end = max(marquee_start.x(), marquee_end.x());
        m_y_end = max(marquee_start.y(), marquee_end.y());

        //make sure marquee selection is within window
        m_x_start = max(0, m_x_start);
        m_y_start = max(0, m_y_start);
        m_x_end = min((m_canvas->width() - 1), m_x_end);
        m_y_end = min((m_canvas->height() - 1), m_y_end);

        m_width = m_canvas->width();
        m_height = m_canvas->height();

    } else {
        //if there is no marquee selection, choose the entire canvas
        m_x_start = 0;
        m_y_start = 0;

        //check for a null canvas
        if (m_canvas != NULL) {
            m_x_end = canvas->width() - 1;
            m_y_end = canvas->height() - 1;
            m_width = canvas->width();
            m_height = canvas->height();
        } else {
            m_x_end = 0;
            m_y_end = 0;
            m_width = 0;
            m_height = 0;
        }
    }

    if (canvas != NULL) {
        m_pix = m_canvas->data();
   }
}

Filter::~Filter() {

}

void Filter::paint() {

}

void Filter::convolve(BGRA* pix, int width, int height,
                      int x_start, int x_end,
                      int y_start, int y_end,
                      float* kernel, int k_size) {
    int radius = k_size/2; //automatically takes floor
    int index;
    float b_accum, g_accum, r_accum;
    int swap_width = x_end - x_start + 1;
    int swap_height = y_end - y_start + 1;
    int sx_index, s_index;
    float weight;

    BGRA* swap = new BGRA[swap_width * swap_height];

    //convolve in the x direction
    for (int y = 0; y < swap_height; y++) {
        index = swap_width * y;
        for (int x = 0; x < swap_width; x++) {

            b_accum = 0, g_accum = 0, r_accum = 0;
            weight = 0;

            //get the weights of the neighboring pixels
            for (int k = 0; k < k_size; k++) {

                //the index in the swap array, x direction
                sx_index = min(max(0, x + k - radius), swap_width);
                s_index = y * swap_width + sx_index;
                b_accum += pix[s_index].b * kernel[k];
                g_accum += pix[s_index].g * kernel[k];
                r_accum += pix[s_index].r * kernel[k];
                weight += kernel[k];
            }

            swap[index].b = min(255, (int) max(b_accum/weight, 0.f));
            swap[index].g = min(255, (int) max(g_accum/weight, 0.f));
            swap[index].r = min(255, (int) max(r_accum/weight, 0.f));
            swap[index].a = 255;

            index++;
        }
    }

    //convolve in the x direction
    for (int y = 0; y < swap_height; y++) {
        index = swap_width * y;
        for (int x = 0; x < swap_width; x++) {

            b_accum = 0, g_accum = 0, r_accum = 0;
            weight = 0;

            //get the weights of the neighboring pixels
            for (int k = 0; k < k_size; k++) {

                //the index in the swap array, x direction
                sx_index = min(max(0, x + k - radius), swap_width);
                s_index = y * swap_width + sx_index;
                b_accum += pix[s_index].b * kernel[k];
                g_accum += pix[s_index].g * kernel[k];
                r_accum += pix[s_index].r * kernel[k];
                weight += kernel[k];
            }

            swap[index].b = min(255, (int) max(b_accum/weight, 0.f));
            swap[index].g = min(255, (int) max(g_accum/weight, 0.f));
            swap[index].r = min(255, (int) max(r_accum/weight, 0.f));
            swap[index].a = 255;

            index++;
        }
    }

    memcpy(pix, swap, sizeof(BGRA) * (width * height));
    safeDeleteArray(swap);
}

void Filter::convolve(float *intensity, int width, int height,
                      float *kernel, int k_size) {

    //assumes zero-based indexing
    int radius = k_size/2;
    int index, s_index, sx_index;
    float accum, weight;
    float* swap = new float[width * height];

    //convolve in the x direction
    for (int y= 0; y < height; y++) {
        index = y * width;
        for (int x = 0; x < width; x++) {

            accum = 0, weight = 0;
            //get the weights of the neighboring pixels
            for (int k = -radius; k < radius; k++) {

                //the index in the swap array, x direction
                sx_index = min(max(0, x + k), width);
                s_index = y * width + sx_index;
                accum += intensity[s_index] * kernel[k + radius];
                weight += kernel[k + radius];
            }

            swap[index] = accum;
            index++;
        }
    }

    memcpy(intensity, swap, sizeof(float) * width * height);
    safeDeleteArray(swap);
}

int Filter::colorRange(int color) {
    return min(max(color, 0), 255);
}

void Filter::copyToCanvas(BGRA *canvas, BGRA *src, int src_width) {
    int canv_index, src_index;

    //put the values in m_swap in m_pix
    for (int y = m_y_start; y < m_y_end; y++) {
        canv_index = y * m_width + m_x_start;
        for (int x = m_x_start; x < m_x_end; x++) {

            src_index = (y - m_y_start) * src_width + (x - m_x_start);
            canvas[canv_index] = src[src_index];
            canv_index++;
        }
    }
}
