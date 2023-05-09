#pragma once
#include <iostream>
#include <time.h>
#include <sys/time.h>

#include "common.h"
#include "show_image.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "scene_objects.h"
#include "camera.h"
#include "material.h"
#include "bvh_node.h"
#include "texture.h"
#include "image_texture.h"
#include "diffuse_light.h"
#include "aarectangle.h"
#include "box.h"
#include "transform.h"
#include "aabb.h"
#include "scene_load.h"

namespace lxrt {

class RenderEngine {
public:
    // whether open multithreaded
    #define OPEN_THREAD

    enum class RenderReturnType: uint8_t {
        Default,
        RepeatUse,
        SceneError,
        RenderComplete
    };

    RenderEngine(): render_return_(RenderReturnType::Default), is_render_use_(false), scene_name_(std::string("")) {

    }

    void Render();

    void LoadRenderData(RenderingParameters render_data);

    int GetNowProcessing() const {
        return float(thread_completed) / render_data_.screen_height / render_data_.screen_width * 100.0;
    }

    RenderReturnType GetRenderReturn() const {
        return render_return_;
    }

    void SetSceneName(const std::string& name) {
        scene_name_ = name;
    }

private:
    RenderingParameters render_data_;

    SceneObjects scene_;

    std::string scene_name_;

    std::atomic_int thread_completed = std::atomic_int(0);

    RenderReturnType render_return_;

    bool is_render_use_;

    color3 RayColor(const Ray& ray, const vec3& background, const Hittable& objects, int depth);

    void ThreadFunction(const SceneObjects& scene, const Camera& camera, ShowImage& show_image, int starti, int endi, ShowImage& depth_image, ShowImage& normal_image);

    double GetDepth(const Ray& ray, const Hittable& objects);

    vec3 GetNormal(const Ray& ray, const Hittable& objects);
};

} // namespace lxrt