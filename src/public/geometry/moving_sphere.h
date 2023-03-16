#pragma once
#include "common.h"
#include "hittable.h"

namespace lxrt {

class MovingSphere: public Hittable {
public:
    MovingSphere(): center0_(point3(0)), center1_(point3(0)), time0_(0), time1_(0), radius_(0) { }
    MovingSphere(const point3 &center0, const point3 &center1, double t0, double t1, double r, shared_ptr<Material> m): 
    center0_(center0), center1_(center1), time0_(t0), time1_(t1), radius_(r), material_(m) { }

    virtual bool Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const override;

    virtual bool BoundingBox(double time0, double time1, AABB& output_box) const override;

    vec3 center(double time) const {
        return center0_ + ((time - time0_) / (time1_ - time0_)) * (center1_ - center0_);
    }

private:
    point3 center0_, center1_;
    double time0_, time1_;
    double radius_;
    shared_ptr<Material> material_;
};

} // namespace lxrt