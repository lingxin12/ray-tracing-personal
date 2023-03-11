#pragma once
#include "common.h"
#include "hittable.hpp"

namespace lxrt {

class Sphere: public Hittable {
public:
    Sphere(): center_(point3(0)), radius_(0) { }
    Sphere(const point3 &c, double r, shared_ptr<Material> m): center_(c), radius_(r), material_(m) { }

    virtual bool Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const;

    virtual bool BoundingBox(double time0, double time1, AABB& output_box) const;

private:
    point3 center_;
    double radius_;
    shared_ptr<Material> material_;
};

bool Sphere::Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const {
    auto a = ray.direction().length2();
    auto b = 2.0 * dot(ray.direction(), ray.origin() - center_);
    auto c = dot(ray.origin() - center_, ray.origin() - center_) - radius_ * radius_;
    auto discriminant = b * b - 4.0 * a * c;
    
    auto temp = (-b - sqrt(discriminant)) / (2.0 * a);
    if(t_min < temp && temp < t_max) {
        hit_record.t = temp;
        hit_record.p = ray.at(temp);
        auto outward_normal = (hit_record.p - center_) / radius_;
        hit_record.set_face_normal(ray, outward_normal);
        hit_record.material = material_;
        return true;
    }
    temp = (-b + sqrt(discriminant)) / (2.0 * a);
    if(t_min < temp && temp < t_max) {
        hit_record.t = temp;
        hit_record.p = ray.at(temp);
        auto outward_normal = (hit_record.p - center_) / radius_;
        hit_record.set_face_normal(ray, outward_normal);
        hit_record.material = material_;
        return true;
    }

    return false;
}

bool Sphere::BoundingBox(double time0, double time1, AABB& output_box) const {
    output_box = AABB(
        center_ - vec3(radius_),
        center_ + vec3(radius_));
    return true;
}

} // namespace lxrt