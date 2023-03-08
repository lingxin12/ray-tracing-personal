#pragma once
#include "common.h"
#include "hittable.hpp"

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

    virtual bool Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const;

private:
    std::vector<shared_ptr<Hittable> > objects_;
};

bool SceneObjects::Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const {
    HitRecord temp_hit_record;
    bool hit_anything = false;
    double hit_max = t_max;

    for(const auto &obj: objects_) {
        if(obj->Hit(ray, t_min, hit_max, temp_hit_record)) {
            hit_anything = true;
            hit_record = temp_hit_record;
            hit_max = hit_record.t;
        }
    }

    return hit_anything;
}

} // namespace lxrt