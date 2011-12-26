/*!
   @file   RayScene.h
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010
*/

#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "Scene.h"
#include "Canvas2D.h"
#include "Camera.h"

#define NUM_THREADS 4  //if Multithreading is turned on, number of threads
#define MAX_RAY_DEPTH 2  //if Reflections turned on, max number of times a light ray can recur
#define TEXTURE_BLEND 1.0 // a number between 0.0 and 1.0 of how much the texture should contribute to the diffuse value
#define RAY_EPSILON 1e-4


/*!
  @class RayScene
  @brief Students will implement this class as necessary in the Ray project.
**/

class RayScene : public Scene
{
public:
    RayScene(std::list<scene_obj> *obj_list, CS123SceneGlobalData global, std::list<CS123SceneLightData> *lights, Camera *camera);
    ~RayScene();

    //! Main raytracing method
    virtual void raytrace(Canvas2D* canvas);

protected:
    std::list<CS123SceneLightData> *m_lights;
    std::list<scene_obj> *m_obj_list;
    Camera *m_camera;
    CS123SceneGlobalData m_global;
    Vector4 m_eye;
    Vector4 m_look;

    //width and height of the canvas
    int m_height;
    int m_width;

};

#endif // RAYSCENE_H
