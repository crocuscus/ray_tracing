#pragma once
#include "base_geom.h"
#include "color.h"

struct Material {
    double alpha;
    double reflect;
    double refract;
    Color ambient, diffuse, specular;
    Material() :  alpha(1), reflect(0), refract(0) {
    }
    Material(Color base_color) :
        alpha(1), reflect(0), refract(0) {
        make_standart(base_color);
    }
    void make_standart(const Color& base_color) {
        ambient = base_color / 12;
        diffuse = base_color;
        specular = base_color * 3;
        specular.clamp();
    }
    virtual Color get_diffuse(const Point&) {
        return diffuse;
    }
    virtual Color get_specular(const Point&) {
        return specular;
    }
    virtual ~Material() {}
};

struct MyTexture:Material {
    Color additional;
    Color get_diffuse(const Point &p) override {
        double param = fabs(p.x - floor(p.x));
        return diffuse * param + additional * (1 - param);
    }
    Color get_specular(const Point &p) override {
        double param = fabs(p.x - floor(p.x));
        return specular * param + additional * (1 - param);
    }
    virtual ~MyTexture() override {}
};

struct Figure {
    Material* material;
    string name;

    virtual bool is_intersect(const Ray& ray) const = 0;
    virtual Point intersect_point(const Ray& ray) const = 0;
    virtual Vector normal(const Point& point, const Ray& ray) const = 0;
    virtual Vector get_reflection(const Vector &v, const Vector &norm) const = 0;
    virtual bool contains(const Point& point) const = 0;
    virtual ~Figure() {}
};
