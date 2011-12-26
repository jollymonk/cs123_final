#include "Lichtenstein.h"

#include <iostream>
#include <algorithm>
#include <math.h>
#include "Settings.h"

using namespace std;

Lichtenstein::Lichtenstein(QPoint marquee_start, QPoint marquee_end, Canvas2D* canvas) : Filter(marquee_start, marquee_end, canvas) {

     // if the canvas is not null and there is an area to paint
     if (canvas != NULL) {
         paint();
     }
}

Lichtenstein::~Lichtenstein() {

}

void Lichtenstein::paint() {

    m_dot_radius = 2;
    int diameter = m_dot_radius * 2 + 1;
    int circle_dist = diameter;

    //initialize new canvas
    int index, swap_index;
    int swap_width = m_x_end - m_x_start;
    int swap_height = m_y_end - m_y_start;

    BGRA* m_swap = new BGRA[swap_width * swap_height];

    //fill in the new canvas with white pixels
    BGRA white;

    white.b = 255;
    white.g = 255;
    white.r = 255;
    white.a = 255;

    BGRA black;

    black.b = 0;
    black.g = 0;
    black.r = 0;
    black.a =255;

    for (int i = 0; i < swap_width * swap_height; i++) {
            m_swap[i] = black;
    }

    //create a mask
    int* mask = new int[diameter * diameter];

    // if the distance from the center is more than the radius, the mask will be 0
    // otherwise 1 for constant brush
    for (int y = 0; y < diameter; y++) {
        for (int x = 0; x < diameter; x++) {

            //calculate the index i
            int i = y * diameter + x;

            if (Canvas2D::dist(m_dot_radius + 1, m_dot_radius + 1, x, y) > m_dot_radius) {
                mask[i] = 0;
            } else {
                mask[i] = 1;
            }
        }
    }

    int mask_index;


    //go through swap array and paint circles
    for (int y = 0; y < swap_height; y += circle_dist) {
        for (int x = 0; x < swap_width; x += circle_dist) {

            int mask_x_offset = abs(min(x - m_dot_radius, 0));
            int mask_y_offset = abs(min(y - m_dot_radius, 0));

            swap_index = y * swap_width + x;
            index = (y + m_y_start) * m_width + (x + m_x_start);

            BGRA canvas_color = m_pix[index];

            int row_start = max(0, (x - m_dot_radius));
            int row_end = min(swap_width, (x + m_dot_radius + 1));
            int col_start = max(0, (y - m_dot_radius));
            int col_end = min(swap_height, (y + m_dot_radius + 1));

            for (int i = col_start; i < col_end; i++) {
                for (int j = row_start; j < row_end; j++) {
                    int mask_row = (i - col_start) + mask_y_offset;
                    int mask_col = (j - row_start) + mask_x_offset;
                    mask_index = Canvas2D::getIndex(diameter, mask_row, mask_col);
                    if (mask[mask_index]) {
                        m_swap[i * swap_width + j] = canvas_color;
                    }
                }
            }
        }
    }

    copyToCanvas(m_pix, m_swap, swap_width);

    safeDeleteArray(m_swap);
    safeDeleteArray(mask);

    m_canvas->update();
}
