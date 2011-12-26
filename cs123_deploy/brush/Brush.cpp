/*!
   @file   Brush.cpp
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief  Implementation of common functionality of bitmap brushes.
   You should fill this file in while completing the Brush assignment.

*/

#include "Brush.h"
#include "Canvas2D.h"

#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

unsigned char lerp(unsigned char a, unsigned char b, float percent)
{
    float fa = (float)a / 255.0f;
    float fb = (float)b / 255.0f;
    return (unsigned char)((fa + (fb - fa) * percent) * 255.0f + 0.5f);
}



Brush::Brush() {

    BGRA color;
    color.a = 255;
    color.b = settings.brushBlue;
    color.g = settings.brushGreen;
    color.r = settings.brushRed;

    m_color = color;
    m_flow = settings.brushAlpha;
    m_radius = settings.brushRadius;
    m_mask = NULL;
}


Brush::~Brush()
{
    // @TODO: [BRUSH] Don't forget to delete any memory you allocate. Use delete[] to delete
    //        a whole array. Otherwise you'll just delete the first element!
    //
    //        i.e. delete[] m_mask;
    //

    delete[] m_mask;
}


void Brush::setGreen(int green)
{
    m_color.g = green;
}


void Brush::setRed(int red)
{
    m_color.r = red;
}


void Brush::setBlue(int blue)
{
    m_color.b = blue;
}


void Brush::setFlow(int flow)
{
    m_flow = flow;
    makeMask();
}


void Brush::setRadius(int radius)
{
    m_radius = radius;
    makeMask();
}


void Brush::paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas)
{

    int row_start = max(0, (mouse_x - m_radius));
    int row_end = min(canvas->height(), (mouse_x + m_radius + 1));
    int col_start = max(0, (mouse_y - m_radius));
    int col_end = min(canvas->width(), (mouse_y + m_radius + 1));

    int mask_x_offset = abs(min(mouse_y - m_radius, 0));
    int mask_y_offset = abs(min(mouse_x - m_radius, 0));

    //for every pixel in the mask of our brush, set the pixel color
    BGRA* pix = canvas->data();
    int diameter = m_radius * 2 + 1;
    for (int i = row_start; i < row_end; i++) {
        for (int j = col_start; j < col_end; j++) {

            int canvas_index = Canvas2D::getIndex(canvas->width(), i, j);

            //calculate mask index, taking into account if part of the brush is offscreen
            int mask_row = (i - row_start) + mask_y_offset;
            int mask_col = (j - col_start) + mask_x_offset;
            int mask_index = Canvas2D::getIndex(diameter, mask_row, mask_col);

            //calculate the color values (mask value takes flow into account)
            int blue = m_mask[mask_index] * m_color.b + (1 - m_mask[mask_index]) * pix[canvas_index].b;
            int green = m_mask[mask_index] * m_color.g + (1 - m_mask[mask_index]) * pix[canvas_index].g;
            int red = m_mask[mask_index] * m_color.r + (1 - m_mask[mask_index]) * pix[canvas_index].r;

            pix[canvas_index].b = blue;
            pix[canvas_index].g = green;
            pix[canvas_index].r = red;
            pix[canvas_index].a = 255;
        }
    }
}

float Brush::dist(int x1, int y1, int x2, int y2) {
   int x_dist = x2 - x1;
   int y_dist = y2 - y1;
   float dist = sqrt(x_dist * x_dist + y_dist * y_dist);
   return dist;
}


