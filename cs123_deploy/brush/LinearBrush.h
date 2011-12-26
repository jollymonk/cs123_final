/*!
   @file   LinearBrush.h
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010
*/

#ifndef LinearBrush_Header
#define LinearBrush_Header

#include "Brush.h"

//! This brush should have a mask with linear falloff.
class LinearBrush : public Brush {

    public:

        LinearBrush();

        //! Apply the brush one time to the given canvas centered at (x,y)
        //virtual void paintOnce(int x, int y, Canvas2D* canvas);

    virtual ~LinearBrush();

    protected:
        void makeMask(); /// Constructs the mask for this brush.
};

#endif
