#include <qgl.h>

#include "SceneviewScene.h"
#include "CS123SceneData.h"
#include "Camera.h"
#include "shape/Cone.h"
#include "shape/Cube.h"
#include "shape/Cylinder.h"
#include "shape/Sphere.h"
#include "shape/Shape.h"
#include "Settings.h"
#include "Canvas3D.h"

using namespace std;

SceneviewScene::SceneviewScene() {}

SceneviewScene::~SceneviewScene() {}

void SceneviewScene::setLights(const Camera *follow)
{
    for (int id = 0; id < NUM_OPENGL_LIGHTS; ++id)
        glDisable(GL_LIGHT0 + id);

    //add lights to the scene
    for (list<CS123SceneLightData>::iterator it = m_lights->begin(); it != m_lights->end(); it++) {
        setLight(*it);
    }
}

void SceneviewScene::renderGeometry(bool useMaterials)
{

    int obj_type;
    Matrix4x4 mat_transform;
    double modelview[16];
    CS123SceneMaterial material;
    glEnable(GL_NORMALIZE);

    //for every object
    for (list<scene_obj>::iterator it = m_obj_list->begin(); it != m_obj_list->end(); it++) {

        //get object info
        scene_obj obj = *it;
        obj_type = obj.prim_type;
        mat_transform = obj.mat_transform;
        material = obj.material;

        if (useMaterials) {
            applyMaterial(material);
        }

        //switch to movelview mode and get the modelview matrix
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

        mat_transform = Matrix4x4::transpose(mat_transform);
        mat_transform = mat_transform * Matrix4x4(modelview);

        //fill and load the final transformation matrix (in GL format)
        GLdouble transf_gl[16];
        transf_gl[0] = (GLdouble) mat_transform.a;
        transf_gl[1] = (GLdouble) mat_transform.b;
        transf_gl[2] = (GLdouble) mat_transform.c;
        transf_gl[3] = (GLdouble) mat_transform.d;
        transf_gl[4] = (GLdouble) mat_transform.e;
        transf_gl[5] = (GLdouble) mat_transform.f;
        transf_gl[6] = (GLdouble) mat_transform.g;
        transf_gl[7] = (GLdouble) mat_transform.h;
        transf_gl[8] = (GLdouble) mat_transform.i;
        transf_gl[9] = (GLdouble) mat_transform.j;
        transf_gl[10] = (GLdouble) mat_transform.k;
        transf_gl[11] = (GLdouble) mat_transform.l;
        transf_gl[12] = (GLdouble) mat_transform.m;
        transf_gl[13] = (GLdouble) mat_transform.n;
        transf_gl[14] = (GLdouble) mat_transform.o;
        transf_gl[15] = (GLdouble) mat_transform.p;

        glLoadMatrixd(transf_gl);
        Shape *curr_shape = 0;

        //draw the current object
        assert((obj_type >=0 && obj_type <= 5));
        switch(obj_type) {
        case (PRIMITIVE_CUBE):
            curr_shape = m_cube;
            break;
        case(PRIMITIVE_CONE):
            curr_shape = m_cone;
            break;
        case(PRIMITIVE_CYLINDER):
            curr_shape = m_cylinder;
            break;
        case(PRIMITIVE_SPHERE):
            curr_shape = m_sphere;
            break;
        case(PRIMITIVE_TORUS):
            cout << "Torus has not been created, a sphere is drawn in its place" << endl;
            curr_shape = m_sphere;
            break;
        case(PRIMITIVE_MESH):
            cout << "Mesh has not been created, a sphere is drawn in its place" << endl;
            curr_shape = m_sphere;
            break;
        default:
            curr_shape = m_sphere;
            break;
        }

        curr_shape->draw();
        if (settings.drawNormals) {
            renderNormals(*curr_shape);
        }

        glPopMatrix();
    }
}

void SceneviewScene::renderNormals()
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // Rendering normals is an **optional** part of Sceneview (sorry, not extra credit)
    // You can put all of your renderNormal() calls here if you want to render normals.
    //
    // HINT: Rendering normals can be a helpful debugging tool if things don't look
    // quite right. Your Shapes might be incorrect, and if that's the case, you'll
    // need to go back and fix your buggy shapes code before turning in Sceneview.
    //
}

void SceneviewScene::renderNormals(Shape &shape)
{
    //get the triangle data from the shape
    GL_Triangle* tries = shape.getTries();
    int size = shape.getNumTries();

    //for each triangle, render each normal
    for (int i = 0; i < size; i++)  {
        renderNormal(tries[i].coord1, tries[i].norm1);
        renderNormal(tries[i].coord2, tries[i].norm2);
        renderNormal(tries[i].coord3, tries[i].norm3);
    }
}

void SceneviewScene::settingsChanged()
{

}
