#pragma once
#include "figure.h"

struct Sphere : Figure {
    Point o;
    double r;

    Sphere() : o(), r(0) {}
    Sphere(Point o, double r) : o(o), r(r) {}
    Sphere(Point o, double r, Material y) : o(o), r(r) {
        this->material = new Material;
        *(this->material) = y;
    }
    ~Sphere() {}

    bool is_intersect(const Ray& ray) const;
    Point intersect_point(const Ray& ray) const;
    Vector normal(const Point& point, const Ray& ray) const;
    bool contains(const Point& point) const;
    Vector get_reflection(const Vector& v, const Vector& norm) const;
    bool is_inside(const Point& point) const;
};
