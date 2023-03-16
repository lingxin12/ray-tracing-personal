#pragma once
#include "common.h"
#include "hittable.h"
#include "aabb.h"

namespace lxrt {

class SceneObjects: public Hittable {
public:
    SceneObjects() { }
    SceneObjects(shared_ptr<Hittable> object) {
        objects_.push_back(object);
    }

    void Clear() {
        objects_.clear();
    }
    
    void Add(shared_ptr<Hittable> object) {
        objects_.push_back(object);
    }

    typename std::vector<shared_ptr<Hittable>>::iterator begin() {
        return objects_.begin();
    }

    typename std::vector<shared_ptr<Hittable>>::iterator end() {
        return objects_.end();
    }

    std::vector<shared_ptr<Hittable>>& objects() {
        return objects_;
    }

    virtual bool Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const override;

    virtual bool BoundingBox(double time0, double time1, AABB& output_box) const override;

private:
    std::vector<shared_ptr<Hittable>> objects_;
};

} // namespace lxrt