#pragma once
#include "common.h"
#include "hittable.h"
#include "aabb.h"

namespace lxrt {

////////////////// translate
class Translate: public Hittable {
public:
    Translate(shared_ptr<Hittable> object, const vec3& translate)
        : object_(object), offset_(translate) {}

    virtual bool Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const override;

    virtual bool BoundingBox(double time0, double time1, AABB& output_box) const override;

private:
    shared_ptr<Hittable> object_;
    vec3 offset_;
};

////////////////// rotate
class RotateY: public Hittable {
public:
    RotateY(shared_ptr<Hittable> object, double angle);

    virtual bool Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const override;

    virtual bool BoundingBox(double time0, double time1, AABB& output_box) const override {
            output_box = bbox_;
            return hasbox_;
        }

public:
    shared_ptr<Hittable> object_;
    double sin_theta_;
    double cos_theta_;
    bool hasbox_;
    AABB bbox_;
};

} // namespace lxrt