/*!
   @file   Scene.h
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief  This is a great place to put common data structures that need to be used by
   all your scenes.
*/

#ifndef SCENE_H
#define SCENE_H

#include <CS123Algebra.h>
#include "CS123SceneData.h"
#include "shape/Shape.h"
#include "shape/Cone.h"
#include "shape/Cube.h"
#include "shape/Cylinder.h"
#include "shape/Sphere.h"

class Camera;
class CS123ISceneParser;
class CS123SceneGlobalData;
class CS123SceneLightData;
class CS123SceneNode;
class TextureData;

/**

 @class Scene

 @brief This is the base class for all scenes. Modify this class if you want to provide
 common functionality to all your scenes.

 @author  Evan Wallace (edwallac)
 @author  Ben Herila (bherila)
 @date    9/1/2010

**/

//! Scene global color coefficients
struct scene_obj
{
   Matrix4x4 mat_transform;
   int prim_type;
   CS123SceneMaterial material;
   QImage* texture;

   //only used if there is a texturemap
   int u_repeats;
   int v_repeats;

};

class Scene
{
public:
    Scene();
    virtual ~Scene();

    static void parse(Scene *sceneToFill, CS123ISceneParser *parser);

    ///*! @brief Returns this scene's local camera (a pointer to one of the cameras in SupportCanvas3D). */
    Camera *getCamera() { return m_camera; }

    ///*! @brief Sets this scene's local camera(a pointer to one of the cameras in SupportCanvas3D). */
    void setCamera(Camera *newCamera);

    //! Returns the list of objects in the scene
    virtual std::list<scene_obj>* getObjects();

    //! Returns the list of lights in the scene
    virtual std::list<CS123SceneLightData>* getLights();

    //! Returns the global data for the scene
    virtual CS123SceneGlobalData getGlobal();

protected:

    /*! Adds a primitive to the scene. */
    virtual void addPrimitive(const CS123ScenePrimitive &scenePrimitive, const Matrix4x4 mat_transform);

    /*! Adds a light to the scene. */
    virtual void addLight(const CS123SceneLightData &sceneLight);

    /*! Sets the global data for the scene. */
    virtual void setGlobal(const CS123SceneGlobalData &global);

    void flattenTree(Scene &sceneToFill, CS123ISceneParser &parser, CS123SceneNode &root,
                        Matrix4x4 mat_transform);

    //primitive objects
    Cube *m_cube;
    Cylinder *m_cylinder;
    Sphere *m_sphere;
    Cone *m_cone;

    std::list<CS123SceneLightData> *m_lights;
    std::list<scene_obj> *m_obj_list;
    CS123SceneGlobalData m_global;
    Camera *m_camera;

private:





};

#endif // SCENE_H
