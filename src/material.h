#pragma once
#include "common.h"
#include "hittable.hpp"
#include "texture.hpp"

namespace lxrt {

class Material {
public:
    virtual vec3 emitted(double u, double v, const vec3& p) const {
        return vec3(0, 0, 0);
    }

    virtual bool scatter(const Ray& ray_in, const HitRecord& hit_record, vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian: public Material {
public:
    Lambertian(shared_ptr<Texture> c): albedo_(c) { }

    virtual bool scatter(const Ray& ray_in, const HitRecord& hit_record, vec3& attenuation, Ray& scattered) const override {
        vec3 scatter_direction = hit_record.normal + RandomInUnitSphere();
        scattered = Ray(hit_record.p, scatter_direction, ray_in.time());
        attenuation = albedo_->Value(hit_record.u, hit_record.v, hit_record.p);
        return true;
    }

private:
    shared_ptr<Texture> albedo_;
};

class Metal: public Material {
public:
    Metal(shared_ptr<Texture> c, double f): albedo_(c), fuzziness_(f) { }

    virtual bool scatter(const Ray& ray_in, const HitRecord& hit_record, vec3& attenuation, Ray& scattered) const override {
        vec3 scatter_direction = reflect(ray_in.direction().normalized(), hit_record.normal);
        scattered = Ray(hit_record.p, scatter_direction + fuzziness_ * RandomInUnitSphere(), ray_in.time());
        attenuation = albedo_->Value(hit_record.u, hit_record.v, hit_record.p);
        return dot(scatter_direction, hit_record.normal) > 0;
    }

private:
    shared_ptr<Texture> albedo_;
    double fuzziness_;
};

class Dielectric: public Material {
public:
    Dielectric(double index_of_refraction): index_of_refraction_(index_of_refraction) { }

    virtual bool scatter(const Ray& ray_in, const HitRecord& hit_record, vec3& attenuation, Ray& scattered) const override {
        attenuation = vec3(1, 1, 1);
        double etai_over_etat = hit_record.front_face ? (1.0 / index_of_refraction_) : index_of_refraction_;

        vec3 unit_direction = ray_in.direction().normalized();
        double cos_theta = ffmin(dot(-unit_direction, hit_record.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        if(etai_over_etat * sin_theta > 1.0) {
            vec3 reflected = reflect(unit_direction, hit_record.normal);
            scattered = Ray(hit_record.p, reflected, ray_in.time());
            return true;
        }

        double reflect_probability = schlick(cos_theta, etai_over_etat);
        if(random_double() < reflect_probability) {
            vec3 reflected = reflect(unit_direction, hit_record.normal);
            scattered = Ray(hit_record.p, reflected, ray_in.time());
            return true;
        }

        vec3 refracted = refract(unit_direction, hit_record.normal, etai_over_etat);
        scattered = Ray(hit_record.p, refracted, ray_in.time());
        return true;
    }

private:
    double index_of_refraction_;
};

} // namespace lxrt