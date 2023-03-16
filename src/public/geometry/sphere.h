#pragma once
#include "common.h"
#include "hittable.h"
#include "aabb.h"

namespace lxrt {

class Sphere: public Hittable {
public:
    Sphere(): center_(point3(0)), radius_(0) { }
    Sphere(const point3 &c, double r, shared_ptr<Material> m): center_(c), radius_(r), material_(m) { }

    virtual bool Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const override;

    virtual bool BoundingBox(double time0, double time1, AABB& output_box) const override;

private:
    point3 center_;
    double radius_;
    shared_ptr<Material> material_;

    static void GetSphereUV(const point3& p, double& u, double& v) {
        auto theta = std::acos(-p.y());
        auto phi = std::atan2(-p.z(), p.x()) + kPI;

        u = phi / (2 * kPI);
        v = theta / kPI;
    }
};

} // namespace lxrt