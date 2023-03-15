#pragma once
#include "common.h"
#include "hittable.hpp"

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

bool Translate::Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const {
    Ray moved_ray(ray.origin() - offset_, ray.direction(), ray.time());
    if (!object_->Hit(moved_ray, t_min, t_max, hit_record))
        return false;

    hit_record.p += offset_;
    hit_record.set_face_normal(moved_ray, hit_record.normal);

    return true;
}

bool Translate::BoundingBox(double time0, double time1, AABB& output_box) const {
    if (!object_->BoundingBox(time0, time1, output_box))
        return false;

    output_box = AABB(
        output_box.min() + offset_,
        output_box.max() + offset_);

    return true;
}

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

RotateY::RotateY(shared_ptr<Hittable> object, double angle) : object_(object) {
    auto radians = degrees_to_radians(angle);
    sin_theta_ = sin(radians);
    cos_theta_ = cos(radians);
    hasbox_ = object->BoundingBox(0, 1, bbox_);

    vec3 min( kInfinity,  kInfinity,  kInfinity);
    vec3 max(-kInfinity, -kInfinity, -kInfinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = i * bbox_.max().x() + (1-i) * bbox_.min().x();
                auto y = j * bbox_.max().y() + (1-j) * bbox_.min().y();
                auto z = k * bbox_.max().z() + (1-k) * bbox_.min().z();

                auto newx =  cos_theta_*x + sin_theta_*z;
                auto newz = -sin_theta_*x + cos_theta_*z;

                vec3 tester(newx, y, newz);

                for (int c = 0; c < 3; c++) {
                    min[c] = ffmin(min[c], tester[c]);
                    max[c] = ffmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox_ = AABB(min, max);
}

bool RotateY::Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const {
    vec3 origin = ray.origin();
    vec3 direction = ray.direction();

    origin[0] = cos_theta_ * ray.origin()[0] - sin_theta_ * ray.origin()[2];
    origin[2] = sin_theta_ * ray.origin()[0] + cos_theta_ * ray.origin()[2];

    direction[0] = cos_theta_ * ray.direction()[0] - sin_theta_ * ray.direction()[2];
    direction[2] = sin_theta_ * ray.direction()[0] + cos_theta_ * ray.direction()[2];

    Ray rotated_ray(origin, direction, ray.time());

    if (!object_->Hit(rotated_ray, t_min, t_max, hit_record))
        return false;

    vec3 p = hit_record.p;
    vec3 normal = hit_record.normal;

    p[0] =  cos_theta_ * hit_record.p[0] + sin_theta_ * hit_record.p[2];
    p[2] = -sin_theta_ * hit_record.p[0] + cos_theta_ * hit_record.p[2];

    normal[0] =  cos_theta_ * hit_record.normal[0] + sin_theta_ * hit_record.normal[2];
    normal[2] = -sin_theta_ * hit_record.normal[0] + cos_theta_ * hit_record.normal[2];

    hit_record.p = p;
    hit_record.set_face_normal(rotated_ray, normal);

    return true;
}

} // namespace lxrt