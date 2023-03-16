#pragma once
#include "common.h"
#include "aarectangle.h"
#include "hittable.h"
#include "scene_objects.h"
#include "material.h"
#include "aabb.h"

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

} // namespace lxrt
