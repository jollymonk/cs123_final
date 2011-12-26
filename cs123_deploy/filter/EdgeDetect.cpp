#include "EdgeDetect.h"

#include <iostream>
#include <algorithm>
#include <math.h>
#include "Settings.h"

using namespace std;

EdgeDetect::EdgeDetect(QPoint marquee_start, QPoint marquee_end, Canvas2D* canvas) : Filter(marquee_start, marquee_end, canvas) {

    if (canvas != NULL) {
        paint();
    }
}

EdgeDetect::~EdgeDetect() {

}

void EdgeDetect::paint() {

    int edge_width = m_x_end - m_x_start + 1;
    int edge_height = m_y_end - m_y_start + 1;
    float* swap_gx = new float[edge_width * edge_height];
    float* swap_gy = new float[edge_width * edge_height];
    float intensity;
    int index, s_index;

    for (int y = m_y_start; y <= m_y_end; y++) {
        index = y * m_width + m_x_start;
        for (int x = m_x_start; x <= m_x_end; x++) {

            intensity = .114 * m_pix[index].b +
                              .587 * m_pix[index].g +
                              .299 * m_pix[index].r;

            s_index = (y - m_y_start) * edge_width + (x - m_x_start);
            swap_gx[s_index] = intensity/255.f;
            swap_gy[s_index] = intensity/255.f;
            assert(s_index < (edge_width* edge_height));

            index++;
        }
    }

    float kern_gx1 [3] = {-1, 0, 1};
    float kern_gx2 [3] = {1, 2, 1};
    float kern_gy1 [3] = {1, 2, 1};
    float kern_gy2 [3] = {1, 0, -1};
    int k_size = 3;

    convolve(swap_gx, edge_width, edge_height,
             kern_gx1, k_size);

    convolve(swap_gx, edge_width, edge_height,
             kern_gx2, k_size);

    convolve(swap_gy, edge_width, edge_height,
             kern_gy1, k_size);

    convolve(swap_gy, edge_width, edge_height,
             kern_gy2, k_size);

    for (int y = 0; y < edge_height; y++) {
        s_index = y * edge_width;
        for (int x = 0; x < edge_width; x++) {

            index = (y + m_y_start) * m_width + (x + m_x_start);
            swap_gx[s_index] *= swap_gx[s_index];
            swap_gy[s_index] *= swap_gy[s_index];
            intensity = (sqrt(swap_gx[s_index] + swap_gy[s_index]));
            intensity *= settings.edgeDetectThreshold;
            intensity = colorRange((intensity) * 255);

            m_pix[index] = BGRA(intensity, intensity, intensity, 255);
            s_index++;
        }
    }

    safeDeleteArray(swap_gx);
    safeDeleteArray(swap_gy);
    m_canvas->update();
}
