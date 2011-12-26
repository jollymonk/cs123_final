#include "RayThread.h"
#include <QApplication>
#include "Settings.h"

using namespace std;

RayThread::RayThread(int x_start, int x_end,
                     int y_start, int y_end,
                     Canvas2D *canvas, int thread_id)
{

    m_x_start = x_start;
    m_x_end = x_end;
    m_y_start = y_start;
    m_y_end = y_end;
    m_width = canvas->width();
    m_height = canvas->height();
    m_thread_id = thread_id;
    m_pix = canvas->data();
}

RayThread::~RayThread() {}

void RayThread::setUpScene(list<scene_obj> *objs, list<CS123SceneLightData> *lights,
                           CS123SceneGlobalData global,
                           Cube *cube, Cone *cone, Sphere *sphere, Cylinder *cylinder,
                           Vector4 p_eye, Matrix4x4 film_to_world)
{
    m_obj_list = objs;
    m_lights = lights;
    m_global = global;
    m_cube = cube;
    m_cone = cone;
    m_sphere = sphere;
    m_cylinder = cylinder;
    m_eye = p_eye;
    m_film_to_world = film_to_world;
    m_global_ambient = global.ka;
    m_global_diffuse = global.kd;
    m_global_specular = global.ks;

}

void RayThread::run() {

    //calculate rays, starting with 3D film plane
    REAL film_x = INFINITY, film_y, film_z = -1.0;
    Vector4 p_film, p_world, v_dir;

    //index of point on 2D canvas
    int index;

    //for all rows
    for (int y = m_y_start; y < m_y_end; y++)
    {
        film_y = -1.0 * ((2.0 * (REAL) y)/ (REAL) m_height) + 1.0;

        //for all cols
        for (int x = m_x_start; x < m_x_end; x++)
        {
            index = y * m_width + x;
            assert(index <= m_height * m_width);
            film_x = ((2.0 * (REAL) x)/ (REAL) m_width) - 1.0;

            p_film = Vector4(film_x, film_y, film_z, 1.0);
            p_world = m_film_to_world * p_film;

            //calculate d in ray = p_eye + t * d
            v_dir = p_world - m_eye;
            v_dir = v_dir.getNormalized();

            //cast the ray and get the color to display on the screen
            CS123SceneColor color = castRay(m_eye, v_dir, 0);
            m_pix[index].b = color.b;
            m_pix[index].g = color.g;
            m_pix[index].r = color.r;
        }
    }
}

//***************************************************************
// Cast ray
//***************************************************************

//create ray in method, ray = p_eye + t * v_dir
CS123SceneColor RayThread::castRay(Vector4 p_eye_ws, Vector4 v_dir_ws, int depth)
{
    //ultimate value to return
    CS123SceneColor color;

    //calculate t
    REAL t_final = INFINITY, t_temp = INFINITY;
    scene_obj *nearest_obj = NULL;
    v_dir_ws.w = 0.0;

    //for each object in the scene, calculate the nearest intersection with the object and the normal
    for (list<scene_obj>::iterator obj = m_obj_list->begin(); obj != m_obj_list->end(); obj++) {
        t_temp = calcIntersection(p_eye_ws, v_dir_ws, &(*obj));

        //if it is the nearest value, update current closest values
        if ((t_temp != INFINITY) && ((t_final - t_temp) > RAY_EPSILON) && (t_final > t_temp))
        {
            nearest_obj = &(*obj);
            t_final = t_temp;
        }
    }

    //if an object was hit
    if ((t_final != INFINITY) && (nearest_obj))
    {

        //intersection points and normals, world and obj space
        Matrix4x4 obj_to_world = nearest_obj->mat_transform;
        Matrix4x4 world_to_obj = obj_to_world.getInverse();
        Vector4 p_eye_os = world_to_obj * p_eye_ws;
        Vector4 v_dir_os = world_to_obj * v_dir_ws;
        Vector4 p_intersect_os = p_eye_os + t_final * v_dir_os;
        Vector4 p_intersect_ws = p_eye_ws + t_final  * v_dir_ws;

        //get the normal and calculate the color from light data
        Vector4 v_obj_normal_ws = calcObjNormal(nearest_obj, obj_to_world, v_dir_ws, p_eye_ws, p_intersect_os);
        color = illuminate(&(*nearest_obj), v_dir_ws, v_obj_normal_ws, p_intersect_ws, p_intersect_os, depth);
    } else
    // no object was hit
    {
        color.b = 0.0;
        color.g = 0.0;
        color.r = 0.0;
    }

    return color;
}

//***************************************************************
// Object Calculations
//***************************************************************

//check object for intersection with ray, returns distance of ray to intersection
REAL RayThread::calcIntersection(const Vector4 p_eye_ws, const Vector4 v_dir_ws,
                                 scene_obj *obj)
{
    assert(p_eye_ws.w > 0.0);
    assert(v_dir_ws.w == 0.0);

    REAL t = INFINITY;

    //matrix to move eye point P and d into object space
    Matrix4x4 mat_obj_trans = obj->mat_transform;
    mat_obj_trans = mat_obj_trans.getInverse();
    Vector4 p_eye_obj = mat_obj_trans * p_eye_ws;
    Vector4 v_dir_obj = mat_obj_trans * v_dir_ws;
    int obj_type = obj->prim_type;

    switch (obj_type)
    {
    case PRIMITIVE_CUBE:
        t = m_cube->checkIntersection(p_eye_obj, v_dir_obj);
        break;
    case PRIMITIVE_CONE:
        t = m_cone->checkIntersection(p_eye_obj, v_dir_obj);
        break;
    case PRIMITIVE_CYLINDER:
        t = m_cylinder->checkIntersection(p_eye_obj, v_dir_obj);
        break;
    case PRIMITIVE_SPHERE:
        t = m_sphere->checkIntersection(p_eye_obj, v_dir_obj);
        break;
    default:
        t = INFINITY;
        break;
    }

    return max(t, 0.0);
}

Vector4 RayThread::calcObjNormal(scene_obj *obj, Matrix4x4 obj_to_world,
                                 Vector4 v_dir_ws,
                                 Vector4 p_eye_ws,
                                 Vector4 p_intersect_os)
{
    Vector4 obj_normal_ws;
    Vector4 obj_normal_os;
    assert(p_eye_ws.w > 0.0);
    v_dir_ws.w = 0.0;

    int obj_type = obj->prim_type;

    switch (obj_type)
    {
    case PRIMITIVE_CUBE:
        obj_normal_os = m_cube->getNormal(p_intersect_os);
        break;
    case PRIMITIVE_CONE:
        obj_normal_os = m_cone->getNormal(p_intersect_os);
        break;
    case PRIMITIVE_CYLINDER:
        obj_normal_os = m_cylinder->getNormal(p_intersect_os);
        break;
    case PRIMITIVE_SPHERE:
        obj_normal_os = m_sphere->getNormal(p_intersect_os);
        break;
    default:
        obj_normal_os = Vector4::zero();
        break;
    }

    //put normal in world space
    obj_normal_ws = obj_to_world * obj_normal_os;
    obj_normal_ws = obj_normal_ws.getNormalized();

    return obj_normal_ws;
}

Vector2 RayThread::getObjUV(scene_obj *obj, Vector4 p_intersect_os) {

    Vector2 uv_pair;
    int obj_type = obj->prim_type;

    switch (obj_type)
    {
    case PRIMITIVE_CUBE:
        uv_pair = m_cube->getUV(p_intersect_os);
        break;
    case PRIMITIVE_CONE:
        uv_pair = m_cone->getUV(p_intersect_os);
        break;
    case PRIMITIVE_CYLINDER:
        uv_pair = m_cylinder->getUV(p_intersect_os);
        break;
    case PRIMITIVE_SPHERE:
        uv_pair = m_sphere->getUV(p_intersect_os);
        break;
    default:
        uv_pair = Vector2(0.0, 0.0);
        break;
    }

    return uv_pair;
}

//***************************************************************
// Calculate Lighting
//***************************************************************

CS123SceneColor RayThread::illuminate(scene_obj *obj,
                           Vector4 v_dir_ws,
                           Vector4 v_obj_normal_ws,
                           Vector4 p_intersect_ws,
                           Vector4 p_intersect_os,
                           int depth)
{
    //color to return
    CS123SceneColor color;
    REAL b, g, r;

    CS123SceneMaterial material = obj->material;
    //tot_lights is the total contribution from all lights
    CS123SceneColor diffuse, specular, reflections, lights_tot;
    lights_tot.b = 0.0;
    lights_tot.g = 0.0;
    lights_tot.r = 0.0;
    diffuse.b = 0.0;
    diffuse.g = 0.0;
    diffuse.r = 0.0;
    specular.b = 0.0;
    specular.g = 0.0;
    specular.r = 0.0;

    REAL attenuation = 1.0;

    //light direction vector and dot product of object normal and light direction
    Vector4 v_light_ray_ws;

    //go through all the lights and get data
    for (list<CS123SceneLightData>::iterator light = m_lights->begin(); light != m_lights->end(); light++)
    {   
        //values for this light
        CS123SceneLightData l = *light;
        CS123SceneColor light_color = l.color;

        if (l.type == LIGHT_POINT)
            //ray from the light to intersection pt
            v_light_ray_ws = p_intersect_ws - l.pos;

        else if (l.type == LIGHT_DIRECTIONAL)
            v_light_ray_ws = l.dir;
        else
            v_light_ray_ws = Vector4::zero();

        v_light_ray_ws.w = 0.0;
        REAL light_mag = v_light_ray_ws.getMagnitude();
        v_light_ray_ws = v_light_ray_ws.getNormalized();

        //if the light is not in a shadow, add contribution
        if ((!settings.useShadows || !(isShadowed(&(*obj), l, v_light_ray_ws, p_intersect_ws))) &&
            ((settings.usePointLights && l.type == LIGHT_POINT) ||
             (settings.useDirectionalLights && l.type == LIGHT_DIRECTIONAL)))
        {
            //calculate lighting equations
            attenuation = calcAttenuation(l, light_mag);
            diffuse = calcDiffuse(&(*obj), l, material, v_light_ray_ws,
                                  v_obj_normal_ws, p_intersect_ws, p_intersect_os);
            specular = calcSpecular(l, material, v_dir_ws, v_light_ray_ws,
                                                    v_obj_normal_ws);

            //add the diffuse, reflections and specular
            lights_tot.b = lights_tot.b + light_color.b * attenuation * (diffuse.b + specular.b);
            lights_tot.g = lights_tot.g + light_color.g * attenuation * (diffuse.g + specular.g);
            lights_tot.r = lights_tot.r + light_color.r * attenuation * (diffuse.r + specular.r);

        }
    }

    CS123SceneColor obj_ambient = material.cAmbient;
    REAL b_ambient = obj_ambient.b * m_global_ambient;
    REAL g_ambient = obj_ambient.g * m_global_ambient;
    REAL r_ambient = obj_ambient.r * m_global_ambient;

    reflections = calcReflections(&(*obj), material, v_dir_ws, v_obj_normal_ws, p_intersect_ws, depth);

    b = (b_ambient + lights_tot.b + reflections.b) * 255.0;
    g = (g_ambient + lights_tot.g + reflections.g) * 255.0;
    r = (r_ambient + lights_tot.r + reflections.r) * 255.0;

    b = min(255, max((int) b, 0));
    g = min(255, max((int) g, 0));
    r = min(255, max((int) r, 0));

    color.b = b;
    color.g = g;
    color.r = r;

   return color;
}

//***************************************************************
// Calculate Specular
//***************************************************************

CS123SceneColor RayThread::calcSpecular(CS123SceneLightData light,
                                        CS123SceneMaterial material,
                                        Vector4 v_dir_ws,
                                        Vector4 v_light_ray_ws,
                                        Vector4 v_obj_normal_ws)
{
    CS123SceneColor specular;
    CS123SceneColor obj_specular = material.cSpecular;

    Vector4 v_reflected = 2.0 * v_obj_normal_ws * (v_dir_ws.dot(v_obj_normal_ws))
            - v_dir_ws;
    REAL reflects = pow(max(v_reflected.dot(v_light_ray_ws), 0.0), material.shininess);

    //diffuse contribution from global and object
    specular.b = obj_specular.b * reflects * m_global_specular;
    specular.g = obj_specular.g * reflects * m_global_specular;
    specular.r = obj_specular.r * reflects * m_global_specular;

    return specular;
}

//***************************************************************
// Calculate Diffuse
//***************************************************************

CS123SceneColor RayThread::calcDiffuse(scene_obj *obj, CS123SceneLightData light,
                                       CS123SceneMaterial material,
                                       Vector4 v_light_ray_ws,
                                       Vector4 v_obj_normal_ws,
                                       Vector4 p_intersect_ws,
                                       Vector4 p_intersect_os)
{
    //color to ultimately return
    CS123SceneColor diffuse;
    //temp object and world diffuse color
    CS123SceneColor obj_color;

    CS123SceneColor obj_diffuse = material.cDiffuse;
    QImage *texture = obj->texture;

    REAL norms = v_obj_normal_ws.dot(-1.0 * v_light_ray_ws);
    norms = max(norms, 0.0);

    obj_color.b = obj_diffuse.b * norms;
    obj_color.g = obj_diffuse.g * norms;
    obj_color.r = obj_diffuse.r * norms;

    //calculate diffuse contribution
    if (settings.useTextureMapping && texture)
    {
        //load the image, get texture data
        assert(texture);
        REAL blend = material.blend;
        int text_width = texture->width();
        int text_height = texture->height();

        //get number of material u, v repeats
        REAL u_repeats = obj->u_repeats;
        REAL v_repeats = obj->v_repeats;

        //get object u, v from intersect point
        Vector2 uv_pair = getObjUV(obj, p_intersect_os);
        REAL u = uv_pair.x;
        REAL v = uv_pair.y;

        int s = ((int)(u * u_repeats * (REAL) text_width)) % text_width;
        int t = ((int)(v * v_repeats * (REAL) text_height)) % text_height;
        assert(s >= 0);
        assert(t >= 0);

        //get texture color at coordinate
        QColor color = texture->pixel(s, t);
        REAL texture_blue, texture_green, texture_red;
        texture_blue = color.blueF()* norms / m_global_diffuse;
        texture_green = color.greenF() * norms / m_global_diffuse;
        texture_red = color.redF() * norms / m_global_diffuse;

        //set up a texture blend
        diffuse.b = (blend * texture_blue) +
                ((1.0 - blend) * obj_color.b);
        diffuse.g = (blend * texture_green) +
                ((1.0 - blend) * obj_color.g);
        diffuse.r = (blend * texture_red) +
                ((1.0 - blend) * obj_color.r);
    }
    // no texture, use regular diffuse
    else
    {
        diffuse.b = obj_color.b;
        diffuse.g = obj_color.g;
        diffuse.r = obj_color.r;
    }

    diffuse.b = diffuse.b * m_global_diffuse;
    diffuse.g = diffuse.g * m_global_diffuse;
    diffuse.r = diffuse.r * m_global_diffuse;

    return diffuse;
}

//***************************************************************
// Calculate Reflections
//***************************************************************

CS123SceneColor RayThread::calcReflections(scene_obj *obj,
                                           CS123SceneMaterial material,
                                           Vector4 v_dir_ws,
                                           Vector4 v_obj_normal_ws,
                                           Vector4 p_intersect_ws,
                                           int depth)
{
    //diffuse contribution from global and object
    CS123SceneColor reflections;
    CS123SceneColor obj_reflective = material.cReflective;
    v_dir_ws = v_dir_ws.getNormalized();
    v_obj_normal_ws = v_obj_normal_ws.getNormalized();



    if (settings.useReflection && depth < MAX_RAY_DEPTH)
    {
        Vector4 v_reflected = 2.0 * (v_obj_normal_ws) * (v_obj_normal_ws.dot(-1.0 * v_dir_ws))
                + v_dir_ws;
        v_reflected = v_reflected.getNormalized();

        reflections = castRay(p_intersect_ws, v_reflected, depth + 1);

        reflections.b = min(1.0, (REAL) (reflections.b / 255.0));
        reflections.g = min(1.0, (REAL) (reflections.g / 255.0));
        reflections.r = min(1.0, (REAL) (reflections.r / 255.0));
    }
    //no reflections are used, just return no light
    else
    {
        reflections.b = 0.0;
        reflections.g = 0.0;
        reflections.r = 0.0;
    }

    reflections.b = reflections.b * obj_reflective.b * m_global_specular;
    reflections.g = reflections.g * obj_reflective.g * m_global_specular;
    reflections.r = reflections.r * obj_reflective.r * m_global_specular;

    return reflections;
}

REAL RayThread::calcAttenuation(CS123SceneLightData light, REAL light_mag)
{
    Vector3 funct_atten = light.function;
    REAL a_const = funct_atten.x;
    REAL a_linear = funct_atten.y;
    REAL a_quad = funct_atten.z;

    REAL attenuation = 0;

    if (light.type == LIGHT_POINT)
    {
        attenuation = 1.0 / (a_const + a_linear * light_mag +
                             a_quad * light_mag * light_mag);
        attenuation = min(1.0, attenuation);
    }
    else if (light.type == LIGHT_DIRECTIONAL)
        attenuation = 1.0;

    //not implementing spotlights
    else
        attenuation = 0.0;

    return attenuation;
}

bool RayThread::isShadowed(scene_obj *curr_obj, CS123SceneLightData light, Vector4 v_light_ray_ws, Vector4 p_intersect_ws)
{
    //distance from light to intersection point
    REAL dist = Vector4(light.pos - p_intersect_ws).getMagnitude();
    dist -= RAY_EPSILON;
    if (light.type == LIGHT_POINT && settings.usePointLights)
    {
        //for each object in the scene, calculate the nearest intersection with the object and the normal
        for (list<scene_obj>::iterator obj = m_obj_list->begin(); obj != m_obj_list->end(); obj++)
        {
            if (dist > calcIntersection(light.pos, v_light_ray_ws, &(*obj))) {
                return true;
            }

        }
        return false;
    }

    else if (light.type == LIGHT_DIRECTIONAL && settings.useDirectionalLights) {
        //shoot a ray out from point and loop through obs, if you get an intersection
        //value that is less than infinite, it's occluded
        REAL t_temp;

        //for each object in the scene, calculate the nearest intersection with the object and the normal
        for (list<scene_obj>::iterator obj = m_obj_list->begin(); obj != m_obj_list->end(); obj++) {

            t_temp = calcIntersection(p_intersect_ws, -1.0 * v_light_ray_ws, &(*obj));

            if (t_temp != INFINITY && (t_temp > RAY_EPSILON))
            {
                return true;
            }
        }
        return false;
    }
    return true;
}

