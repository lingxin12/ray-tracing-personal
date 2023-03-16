#include "aarectangle.h"

namespace lxrt {

bool XYRectangle::Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const {
    auto t = (k_ - ray.origin().z()) / ray.direction().z();
    if (t < t_min || t > t_max)
        return false;
    auto x = ray.origin().x() + t * ray.direction().x();
    auto y = ray.origin().y() + t * ray.direction().y();
    if (x < x0_ || x > x1_ || y < y0_ || y > y1_)
        return false;
    hit_record.u = (x-x0_)/(x1_-x0_);
    hit_record.v = (y-y0_)/(y1_-y0_);
    hit_record.t = t;
    vec3 outward_normal = vec3(0, 0, 1);
    hit_record.set_face_normal(ray, outward_normal);
    hit_record.material = material_;
    hit_record.p = ray.at(t);
    return true;
}

 bool XZRectangle::Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const {
    auto t = (k_ - ray.origin().y()) / ray.direction().y();
    if (t < t_min || t > t_max)
        return false;
    auto x = ray.origin().x() + t * ray.direction().x();
    auto z = ray.origin().z() + t * ray.direction().z();
    if (x < x0_ || x > x1_ || z < z0_ || z > z1_)
        return false;
    hit_record.u = (x-x0_)/(x1_-x0_);
    hit_record.v = (z-z0_)/(z1_-z0_);
    hit_record.t = t;
    vec3 outward_normal = vec3(0, 1, 0);
    hit_record.set_face_normal(ray, outward_normal);
    hit_record.material = material_;
    hit_record.p = ray.at(t);
    return true;
}

bool YZRectangle::Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const {
    auto t = (k_ - ray.origin().x()) / ray.direction().x();
    if (t < t_min || t > t_max)
        return false;
    auto y = ray.origin().y() + t * ray.direction().y();
    auto z = ray.origin().z() + t * ray.direction().z();
    if (y < y0_ || y > y1_ || z < z0_ || z > z1_)
        return false;
    hit_record.u = (y-y0_)/(y1_-y0_);
    hit_record.v = (z-z0_)/(z1_-z0_);
    hit_record.t = t;
    vec3 outward_normal = vec3(1, 0, 0);
    hit_record.set_face_normal(ray, outward_normal);
    hit_record.material = material_;
    hit_record.p = ray.at(t);
    return true;
}

} // namespace lxrt