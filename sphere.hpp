#pragma once
#include "common.h"
#include "hittable.hpp"

namespace lxrt {
// class Sphere
class Sphere: public Hittable {
public:
    Sphere(): center_(point3(0)), radius_(0) { }
    Sphere(const point3 &c, double r, shared_ptr<Material> m): center_(c), radius_(r), material_(m) { }

    virtual bool Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const;

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
////////////////////

// class MovingSphere
class MovingSphere: public Hittable {
public:
    MovingSphere(): center0_(point3(0)), center1_(point3(0)), time0_(0), time1_(0), radius_(0) { }
    MovingSphere(const point3 &center0, const point3 &center1, double t0, double t1, double r, shared_ptr<Material> m): 
    center0_(center0), center1_(center1), time0_(t0), time1_(t1), radius_(r), material_(m) { }

    virtual bool Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const;

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

};