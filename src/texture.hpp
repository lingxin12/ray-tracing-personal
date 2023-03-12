#pragma once
#include "common.h"
#include "perlin.hpp"

namespace lxrt {

class Texture {
public:
    virtual color3 Value(double u, double v, const vec3& p) const = 0;
};

class SolidColor: public Texture {
public:
    SolidColor() { }
    SolidColor(color3 c): color_(c) { }
    SolidColor(double red, double green, double blue): SolidColor(color3(red, green, blue)) { }

    virtual color3 Value(double u, double v, const vec3& p) const override {
        return color_;
    }
private:
    color3 color_;
};

class CheckerTexture : public Texture {
    public:
        CheckerTexture() {}

        CheckerTexture(shared_ptr<Texture> even, shared_ptr<Texture> odd)
            : even_(even), odd_(odd) {}

        CheckerTexture(color3 c1, color3 c2)
            : even_(make_shared<SolidColor>(c1)) , odd_(make_shared<SolidColor>(c2)) {}

        virtual color3 Value(double u, double v, const point3& p) const override {
            auto sines = std::sin(10 * p.x()) * std::sin(10 * p.y()) * std::sin(10 * p.z());
            if (sines < 0)
                return odd_->Value(u, v, p);
            else
                return even_->Value(u, v, p);
        }

    public:
        shared_ptr<Texture> odd_;
        shared_ptr<Texture> even_;
};

class NoiseTexture: public Texture {
public:
    NoiseTexture() { }
    NoiseTexture(double sc): scale_(sc) { }

    virtual color3 Value(double u, double v, const vec3& p) const override {
        // return color3(1, 1, 1) * 0.5 * (1.0 + noise_.Turb(scale_ * p));
        return vec3(1,1,1) * 0.5 * (1 + sin(scale_*p.z() + 10*noise_.Turb(p)));
    }

private:
    Perlin noise_;
    double scale_;
};

} // namespace lxrt