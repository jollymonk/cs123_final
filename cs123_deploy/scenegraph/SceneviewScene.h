/*!
   @file   SceneviewScene.h
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010
*/

#ifndef SCENEVIEWSCENE_H
#define SCENEVIEWSCENE_H

#include "OpenGLScene.h"

class Shape;
class Sphere;
class Cone;
class Cube;
class Cylinder;

/**

 @class SceneviewScene

 @brief A complex scene consisting of multiple objects. Students will implement
 this class in the Sceneview assignment.

 Here you will implement your scene graph. The structure is up to you - feel free
 to create new classes and data structures as you see fit. We are providing this
 SceneviewScene class for you to use as a stencil if you like.

 Keep in mind that you'll also be rendering entire scenes in the next two assignments,
 Intersect and Ray. The difference between this assignment and those that follow is
 here, we are using OpenGL to do the rendering. In Intersect and Ray, you will be
 responsible for that.

 @author  Evan Wallace (edwallac)
 @author  Ben Herila (bherila)
 @date    9/4/2010

**/

class SceneviewScene : public OpenGLScene {

public:
    SceneviewScene();
    virtual ~SceneviewScene();

    void settingsChanged();

protected:
    /*! Apply the camera matrices and enable or disable lights as necessary.  These
     two steps cannot be split up because for Shapes the light moves with the camera
     while for Sceneview the camera moves with the scene. */
    virtual void setLights(const Camera *follow);

    /*! Render geometry for Shapes and Sceneview, don't apply materials when
        useMaterials is false because we are drawing the outlines and want them
        colored black. */
    virtual void renderGeometry(bool useMaterials);

    /*! Override this and put all renderNormal() calls in here. Again, rendering
        normals is optional but recommended. */
    virtual void renderNormals();

    virtual void renderNormals(Shape &shape);

};

#endif // SCENEVIEWSCENE_H
