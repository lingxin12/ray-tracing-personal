#pragma once
#include "vec3.h"
#include "ray.h"

namespace lxrt {

class AABB {
public:
    AABB() { }
    AABB(const vec3 &min, const vec3 &max): min_(min), max_(max) { }

    vec3 min() const { return min_; }
    vec3 max() const { return max_; }

    bool Hit(const Ray &ray, double t_min, double t_max) const;

private:
    vec3 min_;
    vec3 max_;
};

AABB SurroundingBox(AABB& box0, AABB& box1);

} // namespace lxrt