#pragma once


#include <cmath>
#include <algorithm>


inline float clamp(float x, float min, float max) {
    return std::max(min, std::min(max, x));
}


template<class T>
T sign(T v) { return v > 0 ? 1 : v < 0 ? -1 : 0; }


enum class Axis { X, Y };


struct Box {
    float x, y, w, h;
    float overlap(Box const& b, Axis axis) const {
        if (x >= b.x + b.w ||
            y >= b.y + b.h ||
            x + w <= b.x ||
            y + h <= b.y) return 0;
        float s, t;
        if (axis == Axis::X) {
            s = b.x + b.w - x;
            t = b.x - w - x;
        }
        else {
            s = b.y + b.h - y;
            t = b.y - h - y;
        }
        return std::abs(s) < std::abs(t) ? s : t;
    }
};

