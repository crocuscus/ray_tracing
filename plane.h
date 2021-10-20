#pragma once
#include "figure.h"
#include "base_geom.h"

static int sgn(double x) {
    return (x > 0) ? 1 : -1;
}

struct Plane : Figure {
    Point p;
    Vector a, b, norm;

    Plane() {}
    Plane(Point a, Point b, Point c) : p(a), a(a, b), b(a, c) {
        get_norm_from_basis();
    }
    Plane(Point p, Vector a, Vector b) : p(p), a(a), b(b) {
        get_norm_from_basis();
    }
    Plane(Point p, Vector norm) : p(p), norm(norm) {
        get_basis_from_normal();
    }
    ~Plane() override {}

    void get_basis_from_normal();
    void get_norm_from_basis();
    static bool is_intersect_plane_line(const Plane& plane, const Line& line);
    static Point point_intersect_plane_line(const Plane& plane, const Line& line);
    int half_plane(const Point& point) const;
    bool is_intersect(const Ray& ray) const override;
    Point intersect_point(const Ray& ray) const override;
    Vector normal(const Point& point, const Ray& ray) const override;
    virtual Vector get_reflection(const Vector &v, const Vector &norm) const;
    bool contains(const Point &point) const override;
};

std::ostream& operator<<(std::ostream &output_stream, Plane a);
