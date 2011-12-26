/*!
   @file   Canvas2D.h
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010
*/

#ifndef CANVAS2D_H
#define CANVAS2D_H

#include "SupportCanvas2D.h"

class RayScene;
class Camera;
class Brush;
class Filter;

/*!
   @class Canvas2D

   @brief 2D canvas that students will implement in the Brush and Filter assignments.
   The same canvas will be used to display raytraced images in the Intersect and Ray
   assignments.

**/
class Canvas2D : public SupportCanvas2D
{
    Q_OBJECT
public:
    Canvas2D();
    virtual ~Canvas2D();

    void setScene(RayScene *scene);

    //! UI will call this from the button on the "Ray" dock
    void renderImage(Camera *camera, int width, int height);

    //! This will be called when the settings have changed
    virtual void settingsChanged();

    //! Convenience method that returns the index of a pixel at index x, y
    static int getIndex(int width, int x, int y);

    //! Convenience method that returns the index of a pixel at index x, y
    static int getRow(int index, int width);

    //! Convenience method that returns the index of a pixel at index x, y
    static int getCol(int index, int width);

    static float dist(int x1, int y1, int x2, int y2);



public slots:
    //! UI will call this from the button on the "Ray" dock
    void cancelRender();

    //! UI will call this from the button on the "Filter" dock
    void filterImage();

protected:
    virtual void paintEvent(QPaintEvent *);  /// overridden from SupportCanvas2D
    virtual void mouseDown(int x, int y);    /// called when the left mouse button is pressed down on the canvas
    virtual void mouseDragged(int x, int y); /// called when the left mouse button is dragged on the canvas
    virtual void mouseUp(int x, int y);      /// called when the left mouse button is released

    //! This will be called when the size of the canvas has been changed
    virtual void notifySizeChanged(int w, int h);

    //! A convenience method that generates a new brush from the current settings
    virtual void updateBrush();

private:

    //TODO: [BRUSH, INTERSECT, RAY] Put your member variables here.

    // *****************************************************************************************
    // ** BRUSH
    // *****************************************************************************************

    Brush *m_brush;

    //the current values of the brush
    int m_brush_num;
    int m_radius;
    int m_flow;
    int m_alpha;
    int m_blue;
    int m_green;
    int m_red;

    // *****************************************************************************************
    // ** Filter
    // *****************************************************************************************

     Filter *m_filter;
     int m_filter_num;

     // *****************************************************************************************
     // ** Intersect, Ray
     // *****************************************************************************************

     RayScene *m_scene;
};

#endif // CANVAS2D_H
