#pragma once
#include "common.h"
#include "hittable.h"
#include "scene_objects.h"
#include "aabb.h"

namespace lxrt {

class BVHNode: public Hittable {
public:
    BVHNode() { }

    BVHNode(SceneObjects& scene, double time0, double time1)
        : BVHNode(scene.objects(), 0, scene.objects().size(), time0, time1) { }

    BVHNode(std::vector<shared_ptr<Hittable>>& objects, size_t start, size_t end, double time0, double time1);

    virtual bool Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const override;

    virtual bool BoundingBox(double time0, double time1, AABB& output_box) const override;

private:
    shared_ptr<Hittable> left;
    shared_ptr<Hittable> right;
    AABB box;
};

} // namespace lxrt