#pragma once
#include <cstdint>
#include <string>
#include <cstdlib>

using std::string;

const double MAX_COLOR = 255;

struct Color {
    double r, g, b;

    Color() : r(0), g(0), b(0) {}
    Color(int r, int g, int b) : r(r / MAX_COLOR), g(g / MAX_COLOR), b(b / MAX_COLOR) {}
    Color(double r, double g, double b) : r(r), g(g), b(b) {}
    Color(const Color &color) : r(color.r), g(color.g), b(color.b) {}

    uint32_t get_32_color() {
        clamp();
        uint32_t r8, g8, b8;
        r8 = uint32_t(r * MAX_COLOR) & 0xFF;
        g8 = uint32_t(g * MAX_COLOR) & 0xFF;
        b8 = uint32_t(b * MAX_COLOR) & 0xFF;
        return b8 | (g8 << 8) | (r8 << 16)  ;
    }

    void clamp() {
        r = std::min(r, 1.);
        r = std::max(r, 0.);
        g = std::min(g, 1.);
        g = std::max(g, 0.);
        b = std::min(b, 1.);
        b = std::max(b, 0.  );
    }

    Color operator *(double alpha) const {
        Color color = Color();
        color.r = r * alpha;
        color.g = g * alpha;
        color.b = b * alpha;
        return color;
    }

    Color operator /(double alpha) const {
        Color color = Color();
        color.r = r / alpha;
        color.g = g / alpha;
        color.b = b / alpha;
        return color;
    }

    Color operator *(Color c) const {
        return Color(c.r * r, c.g * g, c.b * b);
    }

    Color operator +(const Color& c) const {
        return Color(r + c.r, g + c.g, b + c.b);
    }

    Color mix(const Color &c, double alpha) const {
        Color color = Color();
        color.r = r * alpha + c.r * (1.0 - alpha);
        color.g = g * alpha + c.g * (1.0 - alpha);
        color.b = b * alpha + c.b * (1.0 - alpha);
        return color;
    }

    string to_str() const {
        return std::to_string(r) + " " + std::to_string(g)
                + " " + std::to_string(b);
    }
};
