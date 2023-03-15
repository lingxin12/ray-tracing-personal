#pragma once
#include "common.h"
#include "hittable.hpp"

namespace lxrt {

class Rectangle: public Hittable {
public:
    Rectangle(): { }

    virtual bool Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const override;

    virtual bool BoundingBox(double time0, double time1, AABB& output_box) const override;

private:
    
};

bool Rectangle::Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const {
    return false;
}

bool Rectangle::BoundingBox(double time0, double time1, AABB& output_box) const {
    return false;
}

} // namespace lxrt