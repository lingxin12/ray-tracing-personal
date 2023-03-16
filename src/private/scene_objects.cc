#include "scene_objects.h"

namespace lxrt {

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

bool SceneObjects::BoundingBox(double time0, double time1, AABB& output_box) const {
    if(objects_.empty()) return false;

    AABB temp_box;
    bool first_box = true;

    for(const auto& object: objects_) {
        if(!object->BoundingBox(time0, time1, temp_box)) return false;
        output_box = first_box ? temp_box : SurroundingBox(output_box, temp_box);
        first_box = false;
    }
    
    return true;
}

} // namespace lxrt