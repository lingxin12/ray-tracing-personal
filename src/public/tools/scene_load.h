#pragma once
#include "common.h"
#include "scene_objects.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "material.h"
#include "bvh_node.h"
#include "texture.h"
#include "image_texture.h"
#include "diffuse_light.h"
#include "aarectangle.h"
#include "box.h"
#include "transform.h"
#include "aabb.h"

namespace lxrt {

class SceneLoad {
public:
    SceneLoad() = default;

    void Load();

    void GetAllSceneName(std::vector<std::string>& name);

    std::function<void(SceneObjects&)> GetScene(const std::string& SceneName, std::string& SceneMessage);

    bool GetCamera(const std::string& SceneName, RenderingParameters& parameters);

private:
    // Scene name, scene description and scene generate function
    std::unordered_map<std::string,
        std::pair<std::string, std::function<void(SceneObjects&)>>> scenes_;
    
    std::unordered_map<std::string, RenderingParameters> camera_;

    void InOneWeekScene(SceneObjects& scene);
    void InOneWeekLastScene(SceneObjects& scene);
    void ChecherTextureTest(SceneObjects& scene);
    void NoiseTextureTest(SceneObjects& scene);
    void ExternImageTextureTest(SceneObjects& scene);
    void LightTest(SceneObjects& scene);
    void BoxTest(SceneObjects& scene);
    void CornellBoxTest(SceneObjects& scene);
};


// Instance
class SceneLoadObject {
public:
    static std::shared_ptr<SceneLoad> get() {
        static std::shared_ptr<SceneLoad> scene_load = std::make_shared<SceneLoad>();
        return scene_load;
    }

private:
    SceneLoadObject() = default;
    ~SceneLoadObject() = default;

    SceneLoadObject(const SceneLoadObject& scene_load_object) = delete;
    const SceneLoadObject& operator=(const SceneLoadObject& scene_load_object) = delete;
};

} // namespace lxrt