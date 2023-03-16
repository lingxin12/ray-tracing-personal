#pragma once
#include "common.h"
#include "material.h"

namespace lxrt {

class DiffuseLight: public Material {
public:
    DiffuseLight() { }
    DiffuseLight(shared_ptr<Texture> color): emit_(color) { }

    virtual bool scatter(const Ray& ray_in, const HitRecord& hit_record, vec3& attenuation, Ray& scattered) const override {
        return false;
    }

    virtual vec3 emitted(double u, double v, const vec3& p) const override {
        return emit_->Value(u, v, p);
    }

private:
    shared_ptr<Texture> emit_;
};

} // namespace lxrt