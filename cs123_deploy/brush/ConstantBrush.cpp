/*!
   @file   ConstantBrush.cpp
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief  Implementation of a brush with a constant mask distribution.
   You should fill this file in while completing the Brush assignment.

*/

#include "ConstantBrush.h"

ConstantBrush::ConstantBrush() : Brush()
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.

    makeMask();

}


ConstantBrush::~ConstantBrush()
{
    //all variables deleted in superclass

}

void ConstantBrush::makeMask()
{
    // Set up the mask for the Constant brush

    int diameter = (m_radius * 2) + 1;
    m_mask = new float [diameter * diameter];

    // if the distance from the center is more than the radius, the mask will be 0
    // otherwise 1 for constant brush
    for (int row = 0; row < diameter; row++) {
        for (int col = 0; col < diameter; col++) {

            //calculate the index i
            int i = Canvas2D::getIndex(diameter, row, col);

            if (dist(m_radius + 1, m_radius + 1, row, col) > m_radius) {
                m_mask[i] = 0;
            } else {
                m_mask[i] = m_flow/255.0;
            }
        }
    }
}


