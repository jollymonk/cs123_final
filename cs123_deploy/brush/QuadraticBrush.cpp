/*!
   @file   QuadraticBrush.cpp
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief Implementation of a brush with a quadratic mask distribution.
   You should fill this file in while completing the Brush assignment.

*/

#include "QuadraticBrush.h"
#include <iostream>

QuadraticBrush::QuadraticBrush() : Brush()
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}

QuadraticBrush::~QuadraticBrush()
{
    // @TODO: [BRUSH] Delete any resources owned by this brush, so you don't leak memory.

}

void QuadraticBrush::makeMask()
{
    // Set up the mask for the Quadratic brush

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


