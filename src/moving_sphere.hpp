#pragma once
#include "common.h"
#include "hittable.hpp"

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

bool MovingSphere::Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const {
    auto a = ray.direction().length2();
    auto b = 2.0 * dot(ray.direction(), ray.origin() - center(ray.time()));
    auto c = dot(ray.origin() - center(ray.time()), ray.origin() - center(ray.time())) - radius_ * radius_;
    auto discriminant = b * b - 4.0 * a * c;
    
    auto temp = (-b - sqrt(discriminant)) / (2.0 * a);
    if(t_min < temp && temp < t_max) {
        hit_record.t = temp;
        hit_record.p = ray.at(temp);
        auto outward_normal = (hit_record.p - center(ray.time())) / radius_;
        hit_record.set_face_normal(ray, outward_normal);
        hit_record.material = material_;
        return true;
    }
    temp = (-b + sqrt(discriminant)) / (2.0 * a);
    if(t_min < temp && temp < t_max) {
        hit_record.t = temp;
        hit_record.p = ray.at(temp);
        auto outward_normal = (hit_record.p - center(ray.time())) / radius_;
        hit_record.set_face_normal(ray, outward_normal);
        hit_record.material = material_;
        return true;
    }

    return false;
}

bool MovingSphere::BoundingBox(double time0, double time1, AABB& output_box) const {
    AABB box0 = AABB(
        center(time0) - vec3(radius_),
        center(time0) + vec3(radius_));
    AABB box1 = AABB(
        center(time1) - vec3(radius_),
        center(time1) + vec3(radius_));
    output_box = SurroundingBox(box0, box1);
    return true;
}

} // namespace lxrt