#include <cmath>
#include <cstdint>
#include <vector>
#include <iostream>
#include <omp.h>
#include "scene.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"


using namespace std;

vector<Scene> scenes;

double Scene::init_scenes() {

    Color beauty = Color(255, 87, 4);
    Material mat = Material(beauty);
    Sphere* p_sphere = new Sphere(Point(-2, 2, 0), 2, mat);
    p_sphere->name = "shpere";

    Material* p_mat = new Material();
    Triangle* p_triangle = new Triangle(Point(2, 6, 5), Point(2, 0, 5), Point(7, 0, 7));
    p_triangle->material = p_mat;
    p_triangle->material->make_standart(Color(0.5, 0.5, 0.));
    p_triangle->material->reflect = 0;
    p_triangle->name = "triangle";
    Triangle* p_triangle2 = new Triangle(Point(2, 6, 5), Point(-3, 0, 7), Point(2, 0, 5));
    p_triangle2->material = p_mat;
    p_triangle2->material->make_standart(Color(0.5, 0.5, 0.));
    p_triangle2->material->reflect = 0;
    p_triangle2->name = "triangle2";
    Triangle* p_triangle3 = new Triangle(Point(2, 6, 5), Point(-3, 0, 7), Point(2, 0, 9));
    p_triangle3->material = p_mat;
    p_triangle3->material->make_standart(Color(0.5, 0.5, 0.));
    p_triangle3->material->reflect = 0;
    p_triangle3->name = "triangle3";

    MyTexture* plane1_mat = new MyTexture();
    plane1_mat->additional = Color(0., 0., 1.);
    Plane *p_plane = new Plane(Point(0, 0, 0), Vector(0, 1, 0));
    p_plane->material = plane1_mat;
    p_plane->material->reflect = 0.2;
    p_plane->material->make_standart(Color(0.5, 0., 0.25));
    p_plane->name = "plane";

    Material *plane2_mat = new Material();
    *plane2_mat = *p_mat;
    Plane *p_plane2 = new Plane(Point(-7, 0, 0), Vector(1, 0, 0));
    p_plane2->material = plane2_mat;
    p_plane2->material->reflect = 1;
    p_plane2->material->make_standart(Color(0.5, 0., 0.25));
    p_plane2->name = "plane2";

    View look;
    look.eye = Point(0, 7, -17);
    look.topleft = Point(-12, 10, -5);
    look.topright = Point(6, 10 , -5);
    look.bottomleft = Point(-12, -3, -5);

    double ratio = 18.0 / 13.0;

    Light light;
    light.p = Point(5, 10, 0);
    light.c = Color(1., 1., 1.);
    light.diffuse = 100;
    light.specular = 150;

    Scene debug_scene;
    debug_scene.look = look;

    debug_scene.light.push_back(light);

    debug_scene.figure.push_back(p_sphere);
    debug_scene.figure.push_back(p_triangle);
    debug_scene.figure.push_back(p_triangle2);
    debug_scene.figure.push_back(p_plane);
    debug_scene.figure.push_back(p_plane2);
    debug_scene.figure.push_back(p_triangle3);

    scenes.push_back(debug_scene);

    return ratio;
}

void Scene::test_scene() {
    Point p(4.60991, 2.94617, 0.467133);
    cout << figure[0]->contains(p) << endl;
}

QImage Scene::draw_scene(int id, int width, int height, int anti_aliasing ) {
    Scene scene = scenes[id];
    //scene.test_scene();
    Vector dW = Vector(scene.look.topleft, scene.look.topright) / (width * anti_aliasing);
    Vector dH = Vector(scene.look.topleft, scene.look.bottomleft) / (height * anti_aliasing);
    Point topleft = scene.look.topleft + dW * 0.5 + dH * 0.5;
    QImage res(width, height, QImage::Format_RGB32);
    #pragma omp parallel
    #pragma omp for
    for (int h_ind = 0; h_ind < height; ++h_ind) {
        for (int w_ind = 0; w_ind < width; ++w_ind) {
            Color c;
            for (int i = 0; i < anti_aliasing; ++i) {
                for (int j = 0; j < anti_aliasing; ++j) {
                    c = c + scene.get_ray_color(
                        Ray(
                            scene.look.eye,
                            topleft + dW * double(w_ind * anti_aliasing + i) + dH * double(h_ind * anti_aliasing + j)
                        ),
                        3
                    );
                }
            }
            c = c / sqr(anti_aliasing);
            res.setPixel(w_ind, h_ind, c.get_32_color());
        }
    }
    return res;
}

Color Scene::get_ray_color(const Ray& ray, int deep) const {

    if (deep <= 0){
        return Color(0, 0, 0);
    }

     Figure* fig = first_intersection(ray);

     if (fig == nullptr) {
         return Color(0, 0, 0);
     }

     Material* material = fig->material;

     Point intersection = fig->intersect_point(ray);
     Color base_color = calc_illumination(fig, intersection, fig->normal(intersection, ray));
     if (material->reflect > EPS) {
         Vector reflection = fig->get_reflection(ray.dir, fig->normal(intersection, ray)).normal();
         Ray reflection_ray = Ray(intersection + reflection * 0.01, reflection);
         Color reflection_color = get_ray_color(reflection_ray, deep - 1);
         base_color = base_color.mix(reflection_color, 1 - material->reflect);
    }

     return base_color;
}

Figure* Scene::first_intersection(const Ray& ray) const {
    Figure* res = nullptr;
    double closest_dist = 1e50;
    for (auto f = figure.begin(); f != figure.end(); ++f) {
        if ((*f)->is_intersect(ray)) {
            double cur_dist = Point::dist(ray.a, (*f)->intersect_point(ray));
            if (cur_dist < closest_dist && cur_dist > EPS) {
                res = *f;
                closest_dist = cur_dist;
            }
        }
    }
    return res;
}


Color Scene::calc_illumination(const Figure* fig, const Point& point, const Vector& n) const {
     Color res = Color(0, 0, 0);
     Color amb = fig->material->ambient * Light::ambient;

     res = res + amb;

     Vector norm = n.normal();
     Vector to_eye = Vector(point, look.eye).normal();

     for (Light sun : light) {
         Vector to_sun = Vector(point, sun.p);
         Ray ray_to_light = Ray(point + to_sun.normal(), to_sun);
         Figure *f = first_intersection(ray_to_light);
         if (f != nullptr) {
             Point p = f->intersect_point(ray_to_light);
             if (Point::dist(p, point) < Point::dist(sun.p, point)) {
                 continue;
             }
         }
         double distance = to_sun.len2();
         Vector reflect = fig->get_reflection(to_sun, norm).normal();
         to_sun.normalize();
         Color diff = sun.c * Vector::dot_product(norm, to_sun) * fig->material->get_diffuse(point) * sun.diffuse / distance;
         Color spec = sun.c * pow(max(Vector::dot_product(reflect, to_eye), 0.0), 2) * fig->material->get_specular(point) * sun.specular / distance;
         spec.clamp();
         res = res + diff + spec;
     }

     return res;
 }
