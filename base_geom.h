#pragma once
#include <cmath>
#include <iostream>

#ifndef BASE_GEOM
#define BASE_GEOM

#define EPS 1e-3


static double sqr(double x) {
    return x * x;
}


struct Vector;
struct Point;

struct Point {

    double x, y, z;
    Point() : x(0), y(0), z(0) {}
    Point(double x0, double y0, double z0) : x(x0), y(y0), z(z0) {}
    Point(const Vector &v);

    double operator [](size_t index){
        return (index == 0) ? x : (index == 1) ? y : z;
    }

    static double dist (const Point &a, const Point &b) {
        return sqrt(sqr(a.x - b.x) + sqr(a.y - b.y) + sqr(a.z - b.z));
    }
    
    bool operator ==(Point p) {
        return dist(*this, p) < EPS;
    }

    Point operator +(const Vector &v) const;

    Point operator -(const Vector &v) const;
};

std::ostream& operator<<(std::ostream &output_stream, Point a);

struct Vector {
    double x, y, z;
    Vector() {}
    Vector(double x, double y, double z) : x(x), y(y), z(z) {}
    Vector(const Point& a) : x(a.x), y(a.y), z(a.z) {}
    Vector(const Point& a, const Point& b) : x(b.x - a.x), y(b.y - a.y), z(b.z - a.z) {}

    Vector operator +(const Vector &v) const {
        return {x + v.x, y + v.y, z + v.z};
    }

    Vector operator -(const Vector &v) const {
        return {x - v.x, y - v.y, z - v.z};
    }

    Vector operator +=(const Vector &v) {
        *this = *this + v;
        return *this;
    }

    Vector operator -=(const Vector &v) {
        *this = *this - v;
        return *this;
    }

    Vector operator -() const {
        return Vector(-x, -y, -z);
    }

    Vector operator *(double s) const {
        return Vector(x * s, y * s, z * s);
    }

    Vector operator /(double s) const {
        return Vector(x / s, y / s, z / s);
    }

    Vector operator *=(double s) {
        *this = *this * s;
        return *this;
    }

    Vector operator /=(double s) {
        *this = *this / s;
        return *this;
    }

    double len2() const {
        return x * x + y * y + z * z;
    }

    double len() const {
        return sqrt(len2());
    }

    Vector normal() const{
        return (*this / len());
    }

    Vector normalize() {
        *this = this->normal();
        return *this;
    }

    static double dot_product(const Vector &a, const Vector &b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static Vector cross_product(const Vector &a, const Vector &b) {
        return Vector( -a.y * b.z + a.z * b.y, -a.z * b.x + a.x * b.z, - a.x * b.y + a.y * b.x);
    }

    bool is_collinear(const Vector &a) const {
        return cross_product(a, *this).len2() < EPS;
    }

    bool is_co_direct(const Vector &b) const {
        return is_collinear(b) && ((*this - b).len2() < (*this + b).len2());
    }
};

std::ostream& operator<<(std::ostream &output_stream, Vector a);


struct Line {
public:
    Point a;
    Vector dir;

    Line() {}
    Line(const Point& a, const Vector& dir) : a(a), dir(dir) {}
    Line(const Point& a, const Point& b) : a(a), dir(a, b) {}
    virtual ~Line(){}

    double dist(const Point &b) const;
    virtual bool contains(const Point &b) const;
    Point project(const Point &point) const;
};

std::ostream& operator<<(std::ostream &output_stream, Line a);


struct Ray : Line {
    Ray() {}
    Ray(const Point& a, const Vector& dir) {
        this->a = a;
        this->dir = dir;
    }
    Ray(const Point& a, const Point& b) {
        this->a = a;
        this->dir = Vector(a, b);
    }

    bool contains(const Point& b) const override {
        return this->dir.is_co_direct(Vector(this->a, b));
    }

    double dist(const Point& b) const;
};

double get_hade_cos(const Vector &v, const Vector &norm);

Vector get_reflection(const Vector &v, const Vector &norm);

Vector get_refraction(const Vector &v, const Vector &norm, double refraction);

#endif // BASE_GEOM
