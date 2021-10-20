#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

#include "triangle.h"
#include "base_geom.h"

bool Triangle::contains(const Point& point) const {
    Vector one = Vector::cross_product(Vector(a, b), Vector(b, point));
    Vector two = Vector::cross_product(Vector(b, c), Vector(c, point));
    Vector three = Vector::cross_product(Vector(c, a), Vector(a, point));
    return one.is_co_direct(two) && one.is_co_direct(three);
}

bool Triangle::is_intersect(const Ray& ray) const {
    return p.is_intersect(ray) && contains(p.intersect_point(ray));
}

Point Triangle::intersect_point(const Ray& ray) const {
    return p.intersect_point(ray);
}

Vector Triangle::normal(const Point& point, const Ray &ray) const {
    return p.normal(point, ray);
}

Vector Triangle::get_reflection(const Vector &v, const Vector &norm) const {
    return p.get_reflection(v, norm);
}
