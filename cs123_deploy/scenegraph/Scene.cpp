#include "Scene.h"
#include "Camera.h"
#include "CS123SceneData.h"
#include "CS123ISceneParser.h"
#include "CS123Matrix.h"
#include <QtGlobal>
#include "Settings.h"

using namespace std;

Scene::Scene() //!: m_camera(NULL)
{
    //set up lists for lights and objects
    m_lights = new list<CS123SceneLightData>();
    m_obj_list = new list<scene_obj>();

    //set up basic shapes
    m_sphere = new Sphere();
    m_cone = new Cone();
    m_cube = new Cube();
    m_cylinder = new Cylinder();
}

Scene::~Scene() {
    // Do not delete m_camera, it is owned by SupportCanvas3D

    //delete primitive objects
    safeDelete(m_sphere);
    safeDelete(m_cone);
    safeDelete(m_cube);
    safeDelete(m_cylinder);

    //clear object list
    m_lights->clear();
    safeDelete(m_lights);

    //clear texture data from object list
    for (list<scene_obj>::iterator obj = m_obj_list->begin(); obj != m_obj_list->end(); obj++) {
        scene_obj* s_obj = &(*obj);
        QImage *texture = s_obj->texture;
//        safeDelete(texture);
    }
    //clear object list itself
    m_obj_list->clear();
    safeDelete(m_obj_list);
}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser)
{

    //get scene info from parser
    CS123SceneNode *root = parser->getRootNode();
    CS123SceneGlobalData global;
    CS123SceneLightData light;
    int num_lights = parser->getNumLights();
    Matrix4x4 mat_transform = Matrix4x4::identity();

    //flatten parse tree
    sceneToFill->flattenTree(*sceneToFill, *parser, *root,
                             mat_transform);

    //set global data
    parser->getGlobalData(global);
    sceneToFill->setGlobal(global);

    //add lights to scene
    assert(num_lights >= 0);
    for (int i = 0; i < num_lights; i++) {
        parser->getLightData(i, light);
        sceneToFill->addLight(light);
    }
}

void Scene::setCamera(Camera *newCamera)
{
    // Do not delete the old m_camera, it is owned by SupportCanvas3D
    m_camera = newCamera;
}

void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, Matrix4x4 mat_transform)
{
    scene_obj obj;
    obj.prim_type = scenePrimitive.type;
    CS123SceneMaterial material = scenePrimitive.material;

    //add in texture data if there is a texture
    if (material.textureMap->isUsed) {
        QImage *texture = new QImage();
        bool loaded = texture->load((material.textureMap->filename).c_str());
        if (loaded) {
            cout << "Loaded texture " << (material.textureMap->filename).c_str() << endl;
            obj.texture = texture;
            obj.u_repeats = material.textureMap->repeatU;
            obj.v_repeats = material.textureMap->repeatV;
            assert(material.textureMap->isUsed == true);
        } else {
            cout << cout << " Could not load" << (material.textureMap->filename).c_str() << ", not using texture" << endl;
            obj.texture = 0;
            obj.u_repeats = 0;
            obj.v_repeats = 0;
            material.textureMap->isUsed = false;
            assert(material.textureMap->isUsed == false);
        }
    }
    //if no texture, put a null pointer
    else
    {
        obj.texture = 0;
        obj.u_repeats = 0;
        obj.v_repeats = 0;
    }

    obj.material = material;
    obj.mat_transform = mat_transform;

    //add to object list
    m_obj_list->push_back(obj);
}

void Scene::addLight(const CS123SceneLightData &sceneLight) {
    m_lights->push_back(sceneLight);
}

void Scene::setGlobal(const CS123SceneGlobalData &global) {
    m_global = global;
}

void Scene::flattenTree(Scene &sceneToFill, CS123ISceneParser &parser, CS123SceneNode &node, Matrix4x4 mat_transform) {

    vector<CS123SceneNode*> children = node.children;
    vector<CS123ScenePrimitive*> primitives = node.primitives;
    vector<CS123SceneTransformation*> transforms = node.transformations;
    Matrix4x4 mat_scale, mat_rotate, mat_trans;
    Vector4 v_zero = Vector4::zero();
    Vector4 p_origin = Vector4(0, 0, 0, 1); //returns the origin point

    //add transformations to matrix
    for (int i = 0; i < transforms.size(); i++) {

        CS123SceneTransformation* scene_transform = transforms.at(i);

        //initialize matrix to identity
        mat_trans = Matrix4x4::identity();
        mat_scale = Matrix4x4::identity();
        mat_rotate = Matrix4x4::identity();

        //translate, scale, then rotate
        //translate
        Vector4 v_trans = scene_transform->translate;
        if (v_trans != v_zero) {
            mat_trans = getTransMat(v_trans);
            mat_transform *= mat_trans;
        }

        //scale
        Vector4 v_scale = scene_transform->scale;
        if (v_scale != v_zero) {
            mat_scale = getScaleMat(v_scale);
            mat_transform *= mat_scale;
        }

        //rotation vector
        Vector4 v_rot = scene_transform->rotate;
        if (v_rot != v_zero) {
            REAL radians = scene_transform->angle;
            mat_rotate = getRotMat(p_origin, v_rot, radians);
            mat_transform *= mat_rotate;
        }

        Matrix4x4 mat_current = scene_transform->matrix;
        if (!isZeroMat(mat_current)) {
            mat_transform *= mat_current;
        }
    }

    //make object for each primitive
    for (int i = 0; i < primitives.size(); i++) {
        CS123ScenePrimitive *prim = primitives.at(i);
        sceneToFill.addPrimitive(*prim, mat_transform);
    }

    //run flattentree(...) on children
    for (unsigned i = 0; i < children.size(); i++) {
        flattenTree(sceneToFill, parser, *children.at(i), mat_transform);
    }
}

list<scene_obj>* Scene::getObjects() {
    return m_obj_list;
}

list<CS123SceneLightData>* Scene::getLights() {
    return m_lights;
}

CS123SceneGlobalData Scene::getGlobal() {
    return m_global;
}


