#include "base_geom.h"
#include "plane.h"
Point::Point(const Vector &v) : x(v.x), y(v.y), z(v.z) {}

Point Point::operator +(const Vector &v) const {
    return Vector(*this) + v;
}

Point Point::operator -(const Vector &v) const {
    return Vector(*this) - v;
}

//############################## LINE ##################################

double Line::dist(const Point &b) const {
    return Vector::cross_product(Vector(this->a, b), Vector(this->a + dir, b)).len() / dir.len();
}

bool Line::contains(const Point &b) const {
        return dir.is_collinear(Vector(a, b));
    }

Point Line::project(const Point &point) const{
    Plane plane(point, dir);
    return Plane::point_intersect_plane_line(plane, *this);
}

//############################### RAY ##################################

double Ray::dist(const Point &b) const {
    Line line = Line(this->a, this->dir);
    Point projection = line.project(b);
    if (this->contains(projection)) {
        return line.dist(b);
    }
    return Point::dist(a, b);
}

//############################### PLANE ##################################

bool Plane::is_intersect_plane_line(const Plane& plane, const Line& line) {
        return fabs(Vector::dot_product(plane.norm, line.dir)) > EPS;
    }

Point Plane::point_intersect_plane_line(const Plane& plane, const Line& line) {
    double t = Vector::dot_product(Vector(line.a, plane.p), plane.norm.normal()) / Vector::dot_product(line.dir.normal(), plane.norm.normal());
    Point res = line.a + line.dir.normal() * t;
    return res; 
}

void Plane::get_norm_from_basis() {
    norm = Vector::cross_product(a, b);
}

void Plane::get_basis_from_normal() {
    if (fabs(norm.y) > EPS || fabs(norm.z) > EPS)
        a = Vector(0., -norm.z, norm.y);
    else 
        a = Vector(0., 1., 0.);
    b = Vector::cross_product(norm, a);
    if (!Vector::cross_product(a, b).is_co_direct(norm)) {
        Vector c = a;
        a = b;
        b = c;
    }
}

int Plane::half_plane(const Point& point) const {
     return sgn(Vector::dot_product(norm, Vector(p, point)));
 }

Vector Plane::normal(const Point&, const Ray& ray) const {
    if (half_plane(ray.a) > 0)
        return norm;
    return -norm;
}

bool Plane::is_intersect(const Ray &ray) const {
    if (is_intersect_plane_line(*this, Line(ray.a, ray.dir))) {
        if (ray.contains(point_intersect_plane_line(*this, Line(ray.a, ray.dir))))
            return true;
    }
    return false;
}

Point Plane::intersect_point(const Ray &ray) const {
    return point_intersect_plane_line(*this, Line(ray.a, ray.dir));
}

bool Plane::contains(const Point &) const {
    return true;
}

Vector Plane::get_reflection(const Vector &v, const Vector &norm) const {
        return v.normal() - norm.normal() * get_hade_cos(v, norm) * 2;
    }

//##################### OTHER ##############################

double get_hade_cos(const Vector &v, const Vector &norm) {
    return Vector::dot_product(v.normal(), norm.normal());
}

Vector get_refraction(const Vector &v, const Vector &norm, double refraction) {
      return norm * get_hade_cos(v, norm) * (1 - refraction) + v;
  }

std::ostream& operator<<(std::ostream &output_stream, Point a) {
    output_stream << a.x << " " << a.y << " " << a.z;
    return output_stream;
}


std::ostream& operator<<(std::ostream &output_stream, Vector a) {
    output_stream << a.x << " " << a.y << " " << a.z << std::endl;
}

std::ostream& operator<<(std::ostream &output_stream, Line a) {
     output_stream << "line: point" << a.a << ", direction:" << a.dir << std::endl;
}

std::ostream& operator<<(std::ostream &output_stream, Plane a) {
     output_stream << "plane: point " << a.p << ", direction: " << a.a << ", " << a.b << " normal: " << a.norm << std::endl;
}
