/*!
   @file   QuadraticBrush.h
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010
*/

#ifndef QuadraticBrush_Header
#define QuadraticBrush_Header

#include "Brush.h"

//! This brush should have a mask with $1/r^2$ falloff.
class QuadraticBrush : public Brush {

    public:

        QuadraticBrush();

        //! Apply the brush one time to the given canvas centered at (x,y)
        //virtual void paintOnce(int x, int y, Canvas2D* canvas);

    virtual ~QuadraticBrush();

    protected:
        void makeMask(); /// Constructs the mask for this brush.
};

#endif
