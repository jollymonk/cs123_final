/**

  CS123 2-dimensional canvas. Contains support code necessary for Brush,
  Filter, Intersect, and Ray.

  YOU WILL NEED TO FILL THIS IN!

  Please contact the authors if there are any bugs.

  @date   2010-08-24
  @author Evan Wallace (edwallac)
  @author Ben Herila (ben@herila.net)

**/

#include <math.h>
#include <assert.h>
#include "Canvas2D.h"
#include "Canvas3D.h"
#include "Settings.h"
#include "RayScene.h"
#include "SupportCanvas2D.h"

#include <QPainter>
#include "brush/Brush.h"
#include "brush/ConstantBrush.h"
#include "brush/LinearBrush.h"
#include "brush/QuadraticBrush.h"
#include "brush/SmudgeBrush.h"

#include "filter/Filter.h"
#include "filter/Blur.h"
#include "filter/EdgeDetect.h"
#include "filter/Grayscale.h"
#include "filter/Invert.h"
#include "filter/Scale.h"
#include "filter/Sharpen.h"
#include "filter/Lichtenstein.h"

Canvas2D::Canvas2D() : SupportCanvas2D() {

    m_scene = NULL;

    //initialize brush settings
    m_brush_num = settings.brushType;
    m_radius = settings.brushRadius;
    m_flow = settings.brushAlpha;
    m_blue = settings.brushBlue;
    m_green = settings.brushGreen;
    m_red = settings.brushRed;

    BGRA color;
    color.b = settings.brushBlue;
    color.g = settings.brushGreen;
    color.r = settings.brushRed;
    color.a = 255;

    m_filter_num = settings.filterType;

    //create initial brush
    switch (settings.brushType) {
    case BRUSH_SOLID:
        m_brush = new ConstantBrush();
        break;
    case BRUSH_LINEAR:
        m_brush = new LinearBrush();
        break;
    case BRUSH_QUADRATIC:
        m_brush = new QuadraticBrush();
        break;
    case BRUSH_SMUDGE:
        m_brush = new SmudgeBrush();
        break;
    default:
        m_brush = new ConstantBrush();
        break;
    //case BRUSH_SPECIAL_1:
        //cout << "Special brush 1 has not been created yet" << endl;
        //break;
    //case BRUSH_SPECIAL_2:
        //cout << "Special brush 2 has not been created yet" << endl;
        //break;
    }

    //create some initial filter
    m_filter = new Invert(marqueeStart(), marqueeStop(), NULL);
}

Canvas2D::~Canvas2D() {
    safeDelete(m_scene);
    safeDelete(m_brush);
    safeDelete(m_filter);

}

// This is called when the canvas size is changed. You can change the canvas size
// by calling resize(...). You probably won't need to fill this in, but you can
// if you want to.
void Canvas2D::notifySizeChanged(int w, int h) {

}

void Canvas2D::paintEvent(QPaintEvent *e) {
    // You probably won't need to fill this in, but you can if you want to override
    // any painting events for the 2D canvas. For now, we simply call the superclass.

    SupportCanvas2D::paintEvent(e);

}

// *****************************************************************************************
// ** BRUSH
// *****************************************************************************************

void Canvas2D::mouseDown(int x, int y)
{

    BGRA currentColor;
    currentColor.b = settings.brushBlue;
    currentColor.g = settings.brushGreen;
    currentColor.r = settings.brushRed;
    currentColor.a = 255;

    // you're going to need to leave the alpha value on the canvas itself at 255, but
    // you will need to use the actual alpha value to compute the new color of the pixel

}

void Canvas2D::mouseDragged(int x, int y)
{
    m_brush->paintOnce(x, y, this);
}

void Canvas2D::mouseUp(int x, int y)
{

}

// *****************************************************************************************
// ** FILTER
// *****************************************************************************************

void Canvas2D::filterImage()
{
    delete m_filter;

    switch (settings.filterType) {
    case FILTER_INVERT:
        m_filter = new Invert(marqueeStart(), marqueeStop(), this);
        break;
    case FILTER_GRAYSCALE:
        m_filter = new Grayscale(marqueeStart(), marqueeStop(), this);
        break;
    case FILTER_EDGE_DETECT:
        m_filter = new EdgeDetect(marqueeStart(), marqueeStop(), this);
        break;
    case FILTER_BLUR:
        m_filter = new Blur(marqueeStart(), marqueeStop(), this);
        break;
    case FILTER_SCALE:
        m_filter = new Scale(marqueeStart(), marqueeStop(), this);
        break;
    case FILTER_ROTATE:
        cout << "Rotate has not been created yet" << endl;
        break;
    case FILTER_SPECIAL_1:
        m_filter = new Sharpen(marqueeStart(), marqueeStop(), this);
        break;
    case FILTER_SPECIAL_2:
        m_filter = new Lichtenstein(marqueeStart(), marqueeStop(), this);
        break;
    default:
        m_filter = new Invert(marqueeStart(), marqueeStop(), this);
        break;
    //case FILTER_SPECIAL_3:
        //cout << "Filter Special 3 has not been created yet" << endl;
        //break;
    }

}

void Canvas2D::setScene(RayScene *scene)
{
    safeDelete(m_scene);
    m_scene = scene;
}


void Canvas2D::renderImage(Camera *camera, int width, int height)
{
    if (m_scene)
    {

        // If you want the interface to stay responsive, make sure to call
        // QCoreApplication::processEvents() periodically during the rendering

        this->resize(width, height);
        m_scene->setCamera(camera);
        m_scene->raytrace(this);

    }
}

void Canvas2D::cancelRender()
{
    // TODO: cancel the raytracer (optional)
}



void Canvas2D::settingsChanged() {

    //create a new brush if brush type has changed
    if (m_brush_num != settings.brushType) {
        m_brush_num = settings.brushType;
        updateBrush();
    } else if (m_radius != settings.brushRadius) {
        m_radius = settings.brushRadius;
        m_brush->setRadius(settings.brushRadius);
    }  else if (m_flow != settings.brushAlpha) {
        m_flow = settings.brushAlpha;
        m_brush->setFlow(settings.brushAlpha);
    } else if (m_blue != settings.brushBlue) {
        m_blue = settings.brushBlue;
        m_brush->setBlue(settings.brushBlue);
    } else if (m_green != settings.brushGreen) {
        m_green = settings.brushGreen;
        m_brush->setGreen(settings.brushGreen);
    } else if (m_red != settings.brushRed) {
        m_red = settings.brushRed;
        m_brush->setRed(settings.brushRed);
    }

    SupportCanvas2D::settingsChanged();
}

void Canvas2D::updateBrush()
{

    switch (settings.brushType) {
    case BRUSH_SOLID:
        delete m_brush;
        m_brush = new ConstantBrush();
        break;
    case BRUSH_LINEAR:
        delete m_brush;
        m_brush = new LinearBrush();
        break;
    case BRUSH_QUADRATIC:
        delete m_brush;
        m_brush = new QuadraticBrush();
        break;
    case BRUSH_SMUDGE:
        delete m_brush;
        m_brush = new SmudgeBrush();
        break;
    default:
        delete m_brush;
        m_brush = new ConstantBrush();
        break;
    //case BRUSH_SPECIAL_1:
        //cout << "Special brush 1 has not been created yet" << endl;
        //break;
    //case BRUSH_SPECIAL_2:
        //cout << "Special brush 2 has not been created yet" << endl;
        //break;
    }
}

int Canvas2D::getIndex(int width, int x, int y) {
    return y * width + x;
}

int Canvas2D::getRow(int index, int width){
    //automatically takes the floor
    return index/ width;
}

int Canvas2D::getCol(int index, int width) {
    return index % width;
}

float Canvas2D::dist(int x1, int y1, int x2, int y2) {
   int x_dist = x2 - x1;
   int y_dist = y2 - y1;
   float dist = sqrt(x_dist * x_dist + y_dist * y_dist);
   return dist;
}
