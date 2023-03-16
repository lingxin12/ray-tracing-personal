#pragma once
#include "vec3.h"

namespace lxrt {

class Ray {
public:
    Ray(): origin_(vec3(0)), direction_(vec3(0)), time_(0) { }
    Ray(const point3 &o, const vec3 &d, double time): origin_(o), direction_(d), time_(time) { }

    point3 origin() const { return origin_; }

    vec3 direction() const { return direction_; }

    double time() const { return time_; }

    point3 at(double t) const {
        return origin_ + direction_ * t;
    }

private:
    point3 origin_;
    vec3 direction_;
    double time_;
};

} // namespace lxrt