#include "ShapesScene.h"
#include "Settings.h"
#include "Camera.h"
#include <qgl.h>
#include <SupportCanvas3D.h>
#include "Canvas3D.h"

#include "shape/Shape.h"
#include "shape/Cone.h"
#include "shape/Cube.h"
#include "shape/Cylinder.h"
#include "shape/Sphere.h"

Vector4 lightDirection = Vector4(1, -1, -1, 0).getNormalized();

ShapesScene::ShapesScene()
{
    // Use a shiny orange material
    memset(&m_material, 0, sizeof(m_material));
    m_material.cAmbient.r = 0.2f;
    m_material.cAmbient.g = 0.1f;
    m_material.cDiffuse.r = 1;
    m_material.cDiffuse.g = 0.5f;
    m_material.cSpecular.r = m_material.cSpecular.g = m_material.cSpecular.b = 1;
    m_material.shininess = 64;

    // Use a white directional light from the upper left corner
    memset(&m_light, 0, sizeof(m_light));
    m_light.type = LIGHT_DIRECTIONAL;
    m_light.dir = lightDirection;
    m_light.color.r = m_light.color.g = m_light.color.b = 1;
    m_light.id = 0;

    //TODO: [SHAPES] Allocate any additional memory you need...
    m_shape_num = settings.shapeType;
    m_num_stacks = settings.shapeParameter1;
    m_num_slices = settings.shapeParameter2;
    initializeShape();

}

ShapesScene::~ShapesScene()
{
    safeDelete(m_shape);

}

void ShapesScene::setLights(const Camera *follow)
{
    // YOU DON'T NEED TO TOUCH THIS METHOD,
    // unless you want to do fancy lighting...

    // Disable all OpenGL lights so the lights from the previous scene
    // don't appear in the next one.  There are 8 lights in OpenGL,
    // GL_LIGHT0 to GL_LIGHT7, and each GL_LIGHT* is one greater than
    // the previous one.  This means the nth light is GL_LIGHT0 + n.
    for (int id = 0; id < NUM_OPENGL_LIGHTS; ++id)
        glDisable(GL_LIGHT0 + id);

    m_light.dir = lightDirection * follow->getModelviewMatrix().getInverse().getTranspose();

    // Load one light in the upper left (subclasses will have to load lights
    // for sceneview anyway, so they will overwrite this light).
    setLight(m_light);
}

void ShapesScene::renderGeometry(bool useMaterials)
{
    // TODO: [SHAPES] Render a shape based on settings.shapeType.
    //       The sample code provided draws a single triangle to
    //       the screen using OpenGL. If you've chosen to implement
    //       subclasses for your shapes, this would be a good time
    //       to call upon the subclass to draw itself as well.
    //

    // Example shape:


    if (useMaterials)
        applyMaterial(m_material);

    m_shape->draw();

}

void ShapesScene::renderNormals()
{

    //get the triangle data from the shape
    GL_Triangle* tries = m_shape->getTries();
    int size = m_shape->getNumTries();

    //for each triangle, render each normal
    for (int i = 0; i < size; i++)  {
        renderNormal(tries[i].coord1, tries[i].norm1);
        renderNormal(tries[i].coord2, tries[i].norm2);
        renderNormal(tries[i].coord3, tries[i].norm3);
    }
}

void ShapesScene::settingsChanged() {
    if (m_shape_num != settings.shapeType ||
        m_num_stacks != settings.shapeParameter1 ||
        m_num_slices != settings.shapeParameter2) {

        m_shape_num = settings.shapeType;
        m_num_stacks = settings.shapeParameter1;
        m_num_slices = settings.shapeParameter2;
        updateShape();
    }
}

void ShapesScene::initializeShape(){

    m_shape_num = settings.shapeType;
    m_num_stacks = settings.shapeParameter1;
    m_num_slices = settings.shapeParameter2;

    //create initial shape
    switch (m_shape_num) {
    case SHAPE_CUBE:
        m_shape = new Cube();
        break;
    case SHAPE_CONE:
        m_shape = new Cone();
        break;
    case SHAPE_SPHERE:
        m_shape = new Sphere();
        break;
    case SHAPE_CYLINDER:
        m_shape = new Cylinder();
        break;
    default:
        m_shape = new Sphere();
        break;
    //case SHAPE_SPECIAL_1:
        //m_shape = new Cube();
        //cout << "Special shape 1 has not been created yet" << endl;
        //break;
    //case SHAPE_SPECIAL_2:
        //m_shape = new Cube();
        //cout << "Special shape 2 has not been created yet" << endl;
        //break;
    //case SHAPE_SPECIAL_3:
        //m_shape = new Cube();
        //cout << "Special shape 3 has not been created yet" << endl;
        //break;
    }

    m_shape->draw();
}

void ShapesScene::updateShape() {

    switch(settings.shapeType) {
    case SHAPE_CUBE:
        delete m_shape;
        m_shape =  new Cube();
        break;
    case SHAPE_CONE:
        delete m_shape;
        m_shape = new Cone();
        break;
    case SHAPE_SPHERE:
        delete m_shape;
        m_shape = new Sphere();
        break;
    case SHAPE_CYLINDER:
        delete m_shape;
        m_shape = new Cylinder();
        break;
    default:
        delete m_shape;
        m_shape = new Sphere();
        break;
    //case SHAPE_SPECIAL_1:
        //cout << "Special shape 1 has not been created yet" << endl;
        //break;
    //case SHAPE_SPECIAL_2:
        //cout << "Special shape 2 has not been created yet" << endl;
        //break;
    //case SHAPE_SPECIAL_3:
        //cout << "Special shape 3 has not been created yet" << endl;
        //break;
    }

    m_shape->draw();
}

