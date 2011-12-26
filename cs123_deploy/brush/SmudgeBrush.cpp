/*!
   @file   Brush.cpp
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief
     Implementation of a brush that smudges the image on the canvas as
   the mouse is dragged. You should fill this file in for the Brush
   assignment.

*/

#include "SmudgeBrush.h"
#include <iostream>

using namespace std;

SmudgeBrush::SmudgeBrush() : QuadraticBrush()
{
    // @TODO: [BRUSH] Initialize any memory you are going to use here. Hint - you are going to
    //       need to store temporary image data in memory. This might be a good place to
    //       allocate that memory.

    makeMask();

    //initialize the smudge area to 0;
    int diameter = (m_radius * 2) + 1;
    m_smudge_colors = new BGRA[(diameter * diameter)];
    for (int i = 0; i < (diameter * diameter); i++) {

        m_smudge_colors[i].b = 0;
        m_smudge_colors[i].g = 0;
        m_smudge_colors[i].r = 0;
        m_smudge_colors[i].a = 0;
   }
}


SmudgeBrush::~SmudgeBrush()
{
    delete[] m_smudge_colors;

}



//! create a mask with a distribution of your choice (probably want to use quadratic for best results)
void SmudgeBrush::makeMask()
{
    // @TODO: [BRUSH] Set up the mask for your brush here. For this brush you will probably want
    //        to use a quadratic distribution for the best results. Linear or Gaussian would
    //        work too, however. Feel free to paste your code from the Linear or Quadratic brushes
    //        or modify the class inheritance to be able to take advantage of one of those class's
    //        existing implementations. The choice is yours!

    int diameter = (m_radius * 2) + 1;
    m_mask = new float [diameter * diameter];

    for (int row = 0; row < diameter; row++) {
        for (int col = 0; col < diameter; col++) {

            //calculate the index i
            int i = Canvas2D::getIndex(diameter, row, col);
            float distance = dist(m_radius + 1, m_radius + 1, row, col);

            //if the distance from the center is more than the radius, the mask value is 0
            if (distance > m_radius) {
                m_mask[i] = 0;
            }

            //otherwise the mask value is linear function of distance from the center * flow
            else {

                //distribution is a value between 0 and 1 of the linear distance from the center
                float distribution = 1 - (distance / m_radius);
                m_mask[i] = (distribution * distribution) * m_flow/255.0;
            }
        }
    }
}

//! Picks up paint from the canvas before drawing begins.
void SmudgeBrush::pickUpPaint(int mouse_x, int mouse_y, Canvas2D* canvas)
{
    int row_start = max(0, (mouse_y - m_radius));
    int row_end = min(canvas->height(), (mouse_y + m_radius + 1));
    int col_start = max(0, (mouse_x - m_radius));
    int col_end = min(canvas->width(), (mouse_x + m_radius + 1));

    int smudge_x_offset = fabs(min(mouse_x - m_radius, 0));
    int smudge_y_offset = fabs(min(mouse_y - m_radius, 0));

    //for every pixel in the mask of our brush, set the pixel color
    BGRA* pix = canvas->data();
    int diameter = m_radius * 2 + 1;
    for (int i = row_start; i < row_end; i++) {
        for (int j = col_start; j < col_end; j++) {

            int canvas_index = Canvas2D::getIndex(canvas->width(), i, j);

            //calculate mask index, taking into account if part of the brush is offscreen
            int smudge_row = (i - row_start) + smudge_x_offset;
            int smudge_col = (j - col_start) + smudge_y_offset;
            int smudge_index = Canvas2D::getIndex(diameter, smudge_row, smudge_col);

            //get the color values from the canvas
            int blue = pix[canvas_index].b;
            int green = pix[canvas_index].g;
            int red = pix[canvas_index].r;

            m_smudge_colors[smudge_index].b = blue;
            m_smudge_colors[smudge_index].g = green;
            m_smudge_colors[smudge_index].r = red;
        }
    }
}

void SmudgeBrush::paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas) {
    // @TODO: [BRUSH] Here, you'll need to paste down the paint you picked up in
    //        the previous method. Be sure to take the mask into account! You can
    //        ignore the alpha parameter, but you can also use it (smartly) if you
    //        would like to.

    // now pick up paint again...

    BGRA* pix = canvas->data();
    int diameter = m_radius * 2 + 1;

    int row_start = max(0, (mouse_y - m_radius));
    int row_end = min(canvas->height(), (mouse_y + m_radius + 1));
    int col_start = max(0, (mouse_x - m_radius));
    int col_end = min(canvas->width(), (mouse_x + m_radius + 1));

    int mask_x_offset = fabs(min(mouse_x - m_radius, 0));
    int mask_y_offset = fabs(min(mouse_y - m_radius, 0));

    //for every pixel in the mask of our brush, set the pixel color
    for (int i = row_start; i < row_end; i++) {
        for (int j = col_start; j < col_end; j++) {

            int canvas_index = Canvas2D::getIndex(canvas->width(), i, j);

            //calculate mask index, taking into account if part of the brush is offscreen
            int mask_row = (i - row_start) + mask_y_offset;
            int mask_col = (j - col_start) + mask_x_offset;
            int mask_index = Canvas2D::getIndex(diameter, mask_row, mask_col);

            int smudge_blue = m_smudge_colors[mask_index].b;
            int smudge_green = m_smudge_colors[mask_index].g;
            int smudge_red = m_smudge_colors[mask_index].r;


            //calculate the color values (mask value takes flow into account)
            int blue = m_mask[mask_index] * smudge_blue + (1 - m_mask[mask_index]) * pix[canvas_index].b;
            int green = m_mask[mask_index] * smudge_green + (1 - m_mask[mask_index]) * pix[canvas_index].g;
            int red = m_mask[mask_index] * smudge_red + (1 - m_mask[mask_index]) * pix[canvas_index].r;

            pix[canvas_index].b = blue;
            pix[canvas_index].g = green;
            pix[canvas_index].r = red;
        }
    }

    pickUpPaint(mouse_x, mouse_y, canvas);

}
