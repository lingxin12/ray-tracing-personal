#pragma once
#include "common.h"
#include "aarectangle.hpp"
#include "hittable.hpp"
#include "scene_objects.hpp"
#include "material.h"

namespace lxrt {

class Box: public Hittable  {
public:
    Box() {}
    Box(const point3& box_min, const point3& box_max, shared_ptr<Material> material);

    virtual bool Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const override;

    virtual bool BoundingBox(double time0, double time1, AABB& output_box) const override {
        output_box = AABB(box_min_, box_max_);
        return true;
    }

private:
    point3 box_min_;
    point3 box_max_;
    SceneObjects sides_;
};


Box::Box(const point3& box_min, const point3& box_max, shared_ptr<Material> material) {
    box_min_ = box_min;
    box_max_ = box_max;
    auto p0 = box_min;
    auto p1 = box_max;

    sides_.Add(make_shared<XYRectangle>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), material));
    sides_.Add(make_shared<XYRectangle>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), material));

    sides_.Add(make_shared<XZRectangle>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), material));
    sides_.Add(make_shared<XZRectangle>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), material));

    sides_.Add(make_shared<YZRectangle>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), material));
    sides_.Add(make_shared<YZRectangle>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), material));
}

bool Box::Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const {
    return sides_.Hit(ray, t_min, t_max, hit_record);
}

} // namespace lxrt
