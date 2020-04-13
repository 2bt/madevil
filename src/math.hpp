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
    float overlap(Box const& rhs, Axis axis) const {
        if (x >= rhs.x + rhs.w ||
            y >= rhs.y + rhs.h ||
            x + w <= rhs.x ||
            y + h <= rhs.y) return 0;
        float s, t;
        if (axis == Axis::X) {
            s = rhs.x + rhs.w - x;
            t = rhs.x - w - x;
        }
        else {
            s = rhs.y + rhs.h - y;
            t = rhs.y - h - y;
        }
        return std::abs(s) < std::abs(t) ? s : t;
    }
};

