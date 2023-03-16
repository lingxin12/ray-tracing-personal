#include "box.h"

namespace lxrt {

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