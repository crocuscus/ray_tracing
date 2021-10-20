#pragma once
#include <vector>
#include <QImage>
#include "base_geom.h"
#include "figure.h"

using std::vector;

struct Light {
    Point p;
    Color c;
    double specular, diffuse;
    static constexpr double ambient = 1;
};

struct View {
    Point eye;
    Point topleft;
    Point topright;
    Point bottomleft;

    View() {}
};

struct Scene {

    View look;
    vector <Light> light;
    vector <Figure*> figure;

    Scene() {}
    static QImage draw_scene(int id, int width, int height, int anti_aliasing);
    static double init_scenes();
    void test_scene();
    Color get_ray_color(const Ray& ray, int deep) const;
    Figure* first_intersection(const Ray& ray) const;
    Color calc_illumination(const Figure* fig, const Point& point, const Vector& n) const;
};
