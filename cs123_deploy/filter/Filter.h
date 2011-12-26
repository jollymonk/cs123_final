#ifndef FILTER_H
#define FILTER_H

#include <CS123Common.h>
#include <qgl.h>
#include "math/CS123Algebra.h"
#include "Canvas2D.h"

class Filter
{
public:
    Filter(QPoint marqee_start, QPoint marqee_end, Canvas2D* canvas);

    virtual ~Filter();

    virtual void paint();

    //convolves the kernel over the BGRA array
    void convolve(BGRA* pix, int width, int height,
                  int x_start, int x_end,
                  int y_start, int y_end,
                  float* kernel, int radius);

    //! Convolves an array of floats, assumes 0-based indexing
    void convolve(float* intensity, int width, int height,
                  float* kernel, int k_size);

    void copyToCanvas(BGRA* canvas, BGRA* src, int src_width);

    //returns an int of the color within the 0, 255 range
    int colorRange(int color);

protected:
    int m_x_start;
    int m_x_end;
    int m_y_start;
    int m_y_end;
    int m_width;
    int m_height;
    Canvas2D* m_canvas;
    BGRA* m_pix;
};

#endif // FILTER_H
