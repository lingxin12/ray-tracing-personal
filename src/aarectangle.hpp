#pragma once
#include "common.h"
#include "hittable.hpp"

namespace lxrt {

class XYRectangle: public Hittable {
public:
    XYRectangle() { }

    XYRectangle(double x0, double x1, double y0, double y1, double k, shared_ptr<Material> material)
        : x0_(x0), x1_(x1), y0_(y0), y1_(y1), k_(k), material_(material) {};

    virtual bool Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const override;

    virtual bool BoundingBox(double time0, double time1, AABB& output_box) const override {
        output_box =  AABB(vec3(x0_, y0_, k_ - 0.0001), vec3(x1_, y1_, k_ + 0.0001));
        return true;
    }

private:
    shared_ptr<Material> material_;
    double x0_, x1_, y0_, y1_, k_;
};

class XZRectangle: public Hittable {
public:
    XZRectangle() { }

    XZRectangle(double x0, double x1, double z0, double z1, double k, shared_ptr<Material> material)
        : x0_(x0), x1_(x1), z0_(z0), z1_(z1), k_(k), material_(material) {};

    virtual bool Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const override;

    virtual bool BoundingBox(double time0, double time1, AABB& output_box) const override {
        output_box =  AABB(vec3(x0_ , k_ - 0.0001, z0_), vec3(x1_, k_ + 0.0001, z1_));
        return true;
    }

private:
    shared_ptr<Material> material_;
    double x0_, x1_, z0_, z1_, k_;
};

class YZRectangle: public Hittable {
public:
    YZRectangle() {}

    YZRectangle(double y0, double y1, double z0, double z1, double k, shared_ptr<Material> material)
        : y0_(y0), y1_(y1), z0_(z0), z1_(z1), k_(k), material_(material) {};

    virtual bool Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const override;

    virtual bool BoundingBox(double time0, double time1, AABB& output_box) const override {
        output_box =  AABB(vec3(k_ - 0.0001, y0_, z0_), vec3(k_ + 0.0001, y1_, z1_));
        return true;
    }

private:
    shared_ptr<Material> material_;
    double y0_, y1_, z0_, z1_, k_;
};

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