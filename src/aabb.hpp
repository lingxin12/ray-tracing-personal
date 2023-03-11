#pragma once
#include "vec3.hpp"
#include "ray.hpp"

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

inline bool AABB::Hit(const Ray &ray, double t_min, double t_max) const {
    for(int i = 0; i < 3; i ++) {
        auto inv = 1.0f / ray.direction()[i];
        auto t0 = (min()[i] - ray.origin()[i]) * inv;
        auto t1 = (max()[i] - ray.origin()[i]) * inv;
        if(inv < 0.0f) {
            std::swap(t0, t1);
        }
        t_min = ffmax(t_min, t0);
        t_max = ffmin(t_max, t1);
        if(t_max <= t_min) {
            return false;
        }
    }
    return true;
}

AABB SurroundingBox(AABB& box0, AABB& box1) {
    vec3 box_min(ffmin(box0.min()[0], box1.min()[0]),
                 ffmin(box0.min()[1], box1.min()[1]),
                 ffmin(box0.min()[2], box1.min()[2]));
    vec3 box_max(ffmax(box0.max()[0], box1.max()[0]),
                 ffmax(box0.max()[1], box1.max()[1]),
                 ffmax(box0.max()[2], box1.max()[2]));
    return AABB(box_min, box_max);
}

} // namespace lxrt