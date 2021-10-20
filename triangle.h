#pragma once
#include <plane.h>



struct Triangle : Figure {
    Point a, b, c;
    Plane p;

    Triangle() {}
    Triangle(Point a, Point b, Point c) : a(a), b(b), c(c), p(a, b, c) {}
    Triangle(Plane plane) : a(plane.p), b(plane.p + plane.a), c(plane.p + plane.b), p(plane) {}
    Triangle(Point a, Point b, Point c, Material mat) : a(a), b(b), c(c), p(a, b, c) {
        this->material = new Material;
        *(this->material) = mat;
    }

    bool contains(const Point& p) const;
    bool is_intersect(const Ray& ray) const;
    Point intersect_point(const Ray& ray) const;
    Vector normal(const Point& point, const Ray& ray) const;
    Vector get_reflection(const Vector &v, const Vector &norm) const;
};
