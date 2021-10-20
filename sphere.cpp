#include <cmath>
#include <stdexcept>
#include "sphere.h"
#include "base_geom.h"

using namespace std;

bool Sphere::is_intersect(const Ray &ray) const {
    return ray.dist(this->o) < (r - EPS);
}

Point Sphere::intersect_point(const Ray &ray) const {
    Point projection = ray.project(o);
    if (!is_inside(projection))
        throw std::runtime_error("Panic! projection point not inside shpere!");

    double distance = sqrt(sqr(r) - sqr(Point::dist(o, projection)));
    Point first = projection + ray.dir.normal() * distance;
    Point second = projection - ray.dir.normal() * distance;

    if (!contains(first))
        throw std::runtime_error("Panic! intersect with shpere not on the sphere! (first)");
    if (!contains(second))
        throw std::runtime_error("Panic! intersect with shpere not on the sphere! (second)");

    if (ray.contains(second)) {
        //cout << "RETURN SECOND POINT" << endl;
        return second;
    }

    if (ray.contains(first)) {
        //cout << "RETURN FIRST POINT" << endl;
        return first;
    }

    throw std::runtime_error("sphere doesn't intersect ray");
}

Vector Sphere::normal(const Point &point, const Ray &) const {
    return Vector(this->o, point).normal();
}

bool Sphere::contains(const Point &point) const {
    return fabs(Point::dist(o, point) - r) < EPS;
}

bool Sphere::is_inside(const Point &point) const {
    return Point::dist(o, point) < (r + EPS);
}

Vector Sphere::get_reflection(const Vector &v, const Vector &norm) const {
    return v.normal() + norm.normal() * get_hade_cos(v, norm) * 2;
}
