#ifndef RAYTHREAD_H
#define RAYTHREAD_H

#include "RayScene.h"
#include "Scene.h"
#include <QRunnable>


class RayThread : public QRunnable {

public:
    //! Creates a ray thread responsible for section height x width of canvas (thread_id for debugging)
    RayThread(int x_start, int x_end,
              int y_start, int y_end,
              Canvas2D* canvas, int thread_id);

    ~RayThread();

    //! Supplies thread with all necessary world info
    virtual void setUpScene(std::list<scene_obj> *objs,
               std::list<CS123SceneLightData> *lights,
               CS123SceneGlobalData global,
               Cube *cube,
               Cone *cone,
               Sphere *sphere,
               Cylinder *cylinder,
               Vector4 eye,
               Matrix4x4 film_to_world);

    //overridden from QRunnable
    virtual void run();

protected:

    //! Casts a ray from world point
    virtual CS123SceneColor castRay(Vector4 p_eye_ws, Vector4 v_dir_ws, int depth);

    //! Calculates the distance to intersect a ray with a given object
    virtual REAL calcIntersection(const Vector4 p_eye_ws, const Vector4 v_dir_ws,
                                  scene_obj *obj);

    //! Calculates the surface normal of an intersection point on an object
    virtual Vector4 calcObjNormal(scene_obj *obj, Matrix4x4 obj_to_world,
                                  Vector4 v_dir_ws,
                                  Vector4 p_eye_ws,
                                  Vector4 p_intersect_os);

    virtual Vector2 getObjUV(scene_obj *obj, Vector4 p_intersect_os);

    //! Calculates the color of a pixel given the obj, normal, and obj intersection point
    virtual CS123SceneColor illuminate(scene_obj *obj,
                            Vector4 v_dir_ws,
                            Vector4 v_obj_normal_ws,
                            Vector4 p_intersect_ws,
                            Vector4 p_intersect_os,
                            int depth);

    //! Calculates the contributions of reflections
    virtual CS123SceneColor calcReflections(scene_obj *obj,
                                            CS123SceneMaterial material,
                                            Vector4 v_dir_ws,
                                            Vector4 obj_normal_ws,
                                            Vector4 p_intersect_ws,
                                            int depth);

    //! Calculates the contributions of diffuse color
    virtual CS123SceneColor calcDiffuse(scene_obj *obj, CS123SceneLightData light,
                                        CS123SceneMaterial material,
                                        Vector4 v_light_ray_ws,
                                        Vector4 v_obj_normal_ws,
                                        Vector4 p_intersect_ws,
                                        Vector4 p_intersect_os);

    //! Calculates the contributions of diffuse color
    virtual CS123SceneColor calcSpecular(CS123SceneLightData light,
                                         CS123SceneMaterial material,
                                         Vector4 v_dir_ws,
                                         Vector4 v_light_ray_ws,
                                         Vector4 v_obj_normal_ws);

    virtual REAL calcAttenuation(CS123SceneLightData light, REAL light_mag);

    virtual bool isShadowed(scene_obj *obj,
                            CS123SceneLightData light,
                            Vector4 v_light_ray_ws,
                            Vector4 p_intersect_ws);


private:
    int m_height;
    int m_width;
    int m_x_start;
    int m_x_end;
    int m_y_start;
    int m_y_end;
    int m_canv_width;

    BGRA* m_pix;

    //primitive shapes
    Cube* m_cube;
    Cone* m_cone;
    Sphere* m_sphere;
    Cylinder* m_cylinder;

    //scene info
    std::list<CS123SceneLightData> *m_lights;
    std::list<scene_obj> *m_obj_list;
    CS123SceneGlobalData m_global;
    REAL m_global_ambient;
    REAL m_global_diffuse;
    REAL m_global_specular;
    Matrix4x4 m_film_to_world;
    Vector4 m_eye;
    int m_thread_id;
};

#endif // RAYTHREAD_H
