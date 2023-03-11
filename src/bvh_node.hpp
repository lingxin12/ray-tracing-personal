#pragma once
#include "common.h"
#include "hittable.hpp"
#include "scene_objects.hpp"

namespace lxrt {

inline bool box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, Axis axis) {
    AABB box_a;
    AABB box_b;

    if(!a->BoundingBox(0, 0, box_a) || !b->BoundingBox(0, 0, box_b)) {
        std::cerr << "No bounding box in bvh_node constructor.\n";
    }

    return box_a.min()[static_cast<int>(axis)] < box_b.min()[static_cast<int>(axis)];
}

class BVHNode: public Hittable {
public:
    BVHNode() { }

    BVHNode(SceneObjects& scene, double time0, double time1)
        : BVHNode(scene.objects(), 0, scene.objects().size(), time0, time1) { }

    BVHNode(std::vector<shared_ptr<Hittable>>& objects, size_t start, size_t end, double time0, double time1);

    virtual bool Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const;

    virtual bool BoundingBox(double time0, double time1, AABB& output_box) const;

private:
    shared_ptr<Hittable> left;
    shared_ptr<Hittable> right;
    AABB box;
};

BVHNode::BVHNode(std::vector<shared_ptr<Hittable>>& objects, size_t start, size_t end, double time0, double time1) {
    int axis = random_int(0, 2);
    auto comparator = (axis == 0) ? std::bind(box_compare, std::placeholders::_1, std::placeholders::_2, Axis::X)
                    : (axis == 1) ? std::bind(box_compare, std::placeholders::_1, std::placeholders::_2, Axis::Y)
                                  : std::bind(box_compare, std::placeholders::_1, std::placeholders::_2, Axis::Z);
    size_t objects_number = end - start;

    if(objects_number == 1) {
        left = right = objects[start];
    } else if(objects_number == 2) {
        if(comparator(objects[start], objects[start+1])) {
            left = objects[start];
            right = objects[start+1];
        } else {
            left = objects[start+1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);
        auto mid = start + objects_number / 2;
        left = make_shared<BVHNode>(objects, start, mid, time0, time1);
        right = make_shared<BVHNode>(objects, mid, end, time0, time1);
    }

    AABB box_left, box_right;

    if(!left->BoundingBox(time0, time1, box_left) || !right->BoundingBox(time0, time1, box_right)) {
        std::cerr << "No bounding box in bvh_node constructor.\n";
    }

    box = SurroundingBox(box_left, box_right);
}

bool BVHNode::Hit(const Ray &ray, double t_min, double t_max, HitRecord &hit_record) const {
    if(!box.Hit(ray, t_min, t_max)) {
        return false;
    }

    bool left_hit = left->Hit(ray, t_min, t_max, hit_record);
    bool right_hit = right->Hit(ray, t_min, left_hit ? hit_record.t : t_max, hit_record);
    
    return left_hit || right_hit;
}

bool BVHNode::BoundingBox(double time0, double time1, AABB& output_box) const {
    output_box = box;
    return true;
}

} // namespace lxrt