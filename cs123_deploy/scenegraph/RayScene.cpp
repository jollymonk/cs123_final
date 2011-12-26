#include "RayScene.h"
#include "Settings.h"
#include "CS123SceneData.h"
#include "CS123Algebra.h"
#include <QThreadPool>
#include <QApplication>
#include "RayThread.h"
#include "Camera.h"
#include "CamtransCamera.h"

using namespace std;

RayScene::RayScene(list<scene_obj> *obj_list, CS123SceneGlobalData global, list<CS123SceneLightData> *lights, Camera *camera) {

    assert(camera);
    m_obj_list = obj_list;
    m_global = global;
    m_lights = lights;
    m_camera = camera;
    m_eye = m_camera->getPosition();
    m_look = m_camera->getLook();
}

RayScene::~RayScene() {

}

void RayScene::raytrace(Canvas2D* canvas) {

    m_height = canvas->height();
    m_width = canvas->width();

    Matrix4x4 film_to_world = m_camera->getFilmtoWorldMatrix();

    int threads = 1;

    if (settings.useMultiThreading) {
        //parameter for the number of threads, found in RayScene.h line 15
        threads = NUM_THREADS;
    }

    threads = min(max(threads, 1), m_height);

    //create the threadpool
    QThreadPool thread_pool;
    thread_pool.setMaxThreadCount(threads);

    int x_start = 0;
    int x_end = m_width;
    int y_interval = ceil(m_height/(REAL) threads);
    int y_start = 0;
    int y_end = y_interval;

    //make number of threads
    for (int i = 0; i < threads; i++)
    {
        y_end = min(y_end, m_height);

        //create thread and give it world data
        RayThread *thread = new RayThread(x_start, x_end,
                                          y_start, y_end,
                                          canvas, i);
        thread->setUpScene(m_obj_list, m_lights, m_global,
                           m_cube, m_cone, m_sphere, m_cylinder, m_eye, film_to_world);

        //make thread delete upon completion, and start
        thread->setAutoDelete(true);
        thread_pool.start(thread);

        y_start += y_interval;
        y_end += y_interval;
    }

    while (thread_pool.activeThreadCount() > 0) {
        QCoreApplication::processEvents();
    }

    canvas->update();
}
