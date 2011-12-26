#include "Scale.h"

#include <iostream>
#include <algorithm>
#include <math.h>
#include "Settings.h"

using namespace std;

Scale::Scale(QPoint marquee_start, QPoint marquee_end, Canvas2D* canvas) : Filter(marquee_start, marquee_end, canvas) {

    m_scale_x = settings.scaleX;
    m_scale_y = settings.scaleY;

    // if the canvas is not null and there is an area to paint
    if (m_canvas != NULL) {

        if (marquee_start != marquee_end) {
               crop();
        }
        paint();
    }
}

Scale::~Scale() {

}

void Scale::paint() {

//    m_pix = m_canvas->update();

    //initialize variables
    int orig_index, scale_index, swap_index;
    int swap_height = m_height;
    int swap_width = m_scale_x * m_width;

    const BGRA blue = BGRA(255, 0, 0, 255);
    const BGRA red = BGRA(0, 0, 255, 255);

    //create initial array with new width, old height
    BGRA* swap = new BGRA[swap_width * swap_height];

    for (int y = 0; y < swap_height; y++) {
        scale_index = y * swap_width;

        for (int x = 0; x < swap_width; x++) {

            // we are scaling up
            swap[scale_index] = scaleTriangleX(m_pix, x, y, m_width, m_scale_x);
            scale_index++;
        }
    }

    //resize canvas
    m_canvas->resize(swap_width, swap_height);
    m_pix = m_canvas->data();

    //copy all of the pixel data back into the canvas
    memcpy(m_pix, swap, sizeof(BGRA) * (swap_width * swap_height));
    safeDeleteArray(swap);

    swap_height = m_height * m_scale_y;
    m_x_end = swap_width;
    m_y_end = swap_height;

    //create second array with new width, new height
    swap = new BGRA[swap_width * swap_height];
    m_pix = m_canvas->data();

    for (int y = 0; y < swap_height; y++) {
        scale_index = y * swap_width;

        for (int x = 0; x < swap_width; x++) {
            // we are scaling up
            swap[scale_index] = scaleTriangleY(m_pix, x, y, swap_width, m_scale_y);

            scale_index++;
        }
    }

    //resize canvas
    m_canvas->resize(swap_width, swap_height);
    m_pix = m_canvas->data();
    m_x_end = swap_width;
    m_y_end = swap_height;
    m_height = swap_height;

    //copy all of the pixel data back into the canvas
    memcpy(m_pix, swap, sizeof(BGRA) * (swap_width * swap_height));
    safeDeleteArray(swap);
    m_canvas->update();
}

float Scale::filter(float x, float a) {
    x = fabs(x);

    if (a > 1) {
        //scaling up, radius is 1

        if (x > 1) {
            return 0.0;
        } else {
            return 1 - x;
        }

    } else {
        //scaling down, radius is 1/a
        float radius = 1.0/a;

        if (x > radius) {
            return 0.0;
        } else {
            return ((1 - (x/radius))/radius);
        }
    }
}

BGRA Scale::scaleTriangleX(BGRA *orig, int x, int y, int width, float a) {

    BGRA new_pix;
    int index = 0;
    float k = x/a;
    float left, right, g;
    float b_sum = 0, g_sum = 0, r_sum = 0;
    float weight = 0;

    if (a >= 1) {
        left = max(0.0, ceil(k - 1.0));
        right = min((float) floor(k + 1.0), (float) width);
    } else {
        left = max(0.0, ceil(k - 1.0/a));
        right = min((float) floor(k + 1.0/a), (float) width);
    }

    //iterate through scale kernel
    for (float i = left; i <= right; i++) {
        g = filter(i - k, a);

        index =  y * width + left;
        weight += g;
        b_sum += g * orig[index].b;
        g_sum += g * orig[index].g;
        r_sum += g * orig[index].r;
    }

        new_pix.b = colorRange(b_sum/weight);
        new_pix.g = colorRange(g_sum/weight);
        new_pix.r = colorRange(r_sum/weight);


    return new_pix;
}

BGRA Scale::scaleTriangleY(BGRA *orig, int x, int y, int width, float a) {

    BGRA new_pix;
    int index = 0;
    float k = y/a;
    float left, right, g;
    float b_sum = 0, g_sum = 0, r_sum = 0;
    float weight = 0;

    if (a >= 1) {
        left = max(0.0, ceil(k - 1.0));
        right = min((float) floor(k + 1.0), (float) width);
    } else {
        left = max(0.0, ceil(k - 1.0/a));
        right = min((float) floor(k + 1.0/a), (float) width);
    }

    //iterate through scale kernel
    for (float i = left; i <= right; i++) {
        index =  left * width + x;
        g = filter(i - k, a);

        weight += g;
        b_sum += g * orig[index].b;
        g_sum += g * orig[index].g;
        r_sum += g * orig[index].r;
    }

    new_pix.b = colorRange(b_sum/weight);
    new_pix.g = colorRange(g_sum/weight);
    new_pix.r = colorRange(r_sum/weight);

    return new_pix;
}

void Scale::crop() {
    int orig_index, swap_index;
    int crop_width = m_x_end - m_x_start;
    int crop_height = m_y_end - m_y_start;
    BGRA* swap = new BGRA[crop_height * crop_width];

    //copy all of the pixel data into a temp buffer
    for (int y = m_y_start; y < m_y_end; y++) {
        orig_index = y * m_width + m_x_start;
        swap_index = (y - m_y_start) * (m_x_end - m_x_start);

        for (int j = m_x_start; j < m_x_end; j++) {
            swap[swap_index] = m_pix[orig_index];
            orig_index++;
            swap_index++;
        }
    }

    //resize the canvas
    m_canvas->resize((m_x_end - m_x_start), (m_y_end - m_y_start));
    m_pix = m_canvas->data();

    m_x_start = 0, m_y_start = 0;
    m_x_end = m_canvas->width();
    m_y_end = m_canvas->height();
    m_width = m_canvas->width();
    m_height = m_canvas->height();

    copyToCanvas(m_pix, swap, crop_width);

    safeDeleteArray(swap);
}
