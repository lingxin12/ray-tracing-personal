#pragma once
#include "common.h"

namespace lxrt {

class Material;

struct HitRecord {
    double t;
    point3 p;
    vec3 normal;
    bool front_face;
    shared_ptr<Material> material;

    inline void set_face_normal(const Ray &ray, const vec3 &outward_normal) {
        front_face = dot(ray.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual bool Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const = 0;
};

} // namespace lxrt