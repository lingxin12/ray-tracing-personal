#pragma once
#include "common.h"

namespace lxrt {

class Material;

struct HitRecord {
    double t;                          // ray vector parameter
    point3 p;                          // hit point position
    vec3 normal;                       // normal
    bool front_face;                   // is front face
    shared_ptr<Material> material;     // face material
    double u, v;                       // texture coordinate

    inline void set_face_normal(const Ray &ray, const vec3 &outward_normal) {
        front_face = dot(ray.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual bool Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const = 0;

    virtual bool BoundingBox(double time0, double time1, AABB& output_box) const = 0;
};

} // namespace lxrt