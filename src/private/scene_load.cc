#include "scene_load.h"

namespace lxrt {

void SceneLoad::Load() {
    // -------------------- three_boxes --------------------
    scenes_.insert({
        "three_boxes",
        {"ray tracing in one weekend the first half",
        std::bind(&SceneLoad::InOneWeekScene, this, std::placeholders::_1)}
    });
    camera_.insert({
        "three_boxes",
        {0, 0, 0, 20, 0, vec3(-2, 3, 3), vec3(0, 0, -1), vec3(0), 0, 2, 0, 0, 0}
    });
    // -----------------------------------------------------

    // -------------------- many_boxes --------------------
    scenes_.insert({
        "many_boxes",
        {"ray tracing in one weekend and the last scene",
        std::bind(&SceneLoad::InOneWeekLastScene, this, std::placeholders::_1)}
    });
    camera_.insert({
        "many_boxes",
        {0, 0, 0, 20, 0, vec3(-2, 2.5, 5), vec3(0, 0, 4), vec3(0), 0, 2, 0, 0, 0}
    });
    // -----------------------------------------------------

    // -------------------- checher_texture --------------------
    scenes_.insert({
        "checher_texture",
        {"checher texture test",
        std::bind(&SceneLoad::ChecherTextureTest, this, std::placeholders::_1)}
    });
    camera_.insert({
        "checher_texture",
        {0, 0, 0, 20, 0, vec3(0, 0, 10), vec3(0, 0, 0), vec3(0), 0, 2, 0, 0, 0}
    });
    // -----------------------------------------------------

    // -------------------- noise_texture --------------------
    scenes_.insert({
        "noise_texture",
        {"noise texture test",
        std::bind(&SceneLoad::NoiseTextureTest, this, std::placeholders::_1)}
    });
    camera_.insert({
        "noise_texture",
        {0, 0, 0, 20, 0, vec3(5, 5, 10), vec3(0, 0, 0), vec3(0), 0, 2, 0, 0, 0}
    });
    // -----------------------------------------------------

    // -------------------- image_texture --------------------
    scenes_.insert({
        "image_texture",
        {"extern image texture test",
        std::bind(&SceneLoad::ExternImageTextureTest, this, std::placeholders::_1)}
    });
    camera_.insert({
        "image_texture",
        {0, 0, 0, 20, 0, vec3(10, 0, 200), vec3(0, 0, 0), vec3(0), 0, 2, 0, 0, 0}
    });
    // -----------------------------------------------------

    // -------------------- light_tests --------------------
    scenes_.insert({
        "light_test",
        {"light test",
        std::bind(&SceneLoad::LightTest, this, std::placeholders::_1)}
    });
    camera_.insert({
        "light_test",
        {0, 0, 0, 20, 0, vec3(10, 5, 10), vec3(0, 0, 0), vec3(0), 0, 2, 0, 0, 0}
    });
    // -----------------------------------------------------

    // -------------------- box_generate --------------------
    scenes_.insert({
        "box_generate",
        {"box test",
        std::bind(&SceneLoad::BoxTest, this, std::placeholders::_1)}
    });
    camera_.insert({
        "box_generate",
        {0, 0, 0, 20, 0, vec3(10, 5, 10), vec3(0, 0, 0), vec3(0), 0, 2, 0, 0, 0}
    });
    // -----------------------------------------------------

    // -------------------- cornell_box --------------------
    scenes_.insert({
        "cornell_box",
        {"classic scene: cornell box",
        std::bind(&SceneLoad::CornellBoxTest, this, std::placeholders::_1)}
    });
    camera_.insert({
        "cornell_box",
        {0, 0, 0, 40, 0, vec3(278, 278, 800), vec3(278, 278, 0), vec3(0), 0, 2, 0, 0, 0}
    });
    // -----------------------------------------------------

    std::cerr << "All scenes load successfully!" << std::endl;
}

void SceneLoad::GetAllSceneName(std::vector<std::string>& name) {
    name.clear();
    for(auto it = scenes_.begin(); it != scenes_.end(); it ++) {
        name.push_back(it->first);
    }
}

bool SceneLoad::GetCamera(const std::string& SceneName, RenderingParameters& parameters) {
    if(!camera_.count(SceneName)) {
        return false;
    }
    parameters = camera_[SceneName];
    return true;
}

std::function<void(SceneObjects&)> SceneLoad::GetScene(const std::string& SceneName, std::string& SceneMessage) {
    if(!scenes_.count(SceneName)) {
        return NULL;
    }
    auto pair = scenes_[SceneName];
    SceneMessage = pair.first;
    return pair.second;
}

void SceneLoad::InOneWeekScene(SceneObjects& scene) {
    //---------------- ray tracing in one weekend
    scene.Add(make_shared<Sphere>(vec3(0,0,-1), 0.5, make_shared<Lambertian>(vec3(0.7, 0.3, 0.3))));
    scene.Add(make_shared<Sphere>(vec3(0,-100.5,-1), 100, make_shared<Lambertian>(vec3(0.8, 0.8, 0.0))));

    scene.Add(make_shared<Sphere>(vec3(1,0,-1), 0.5, make_shared<Metal>(vec3(0.8, 0.6, 0.2), 0.2)));
    scene.Add(make_shared<Sphere>(vec3(-1,0,-1), 0.5, make_shared<Dielectric>(1.5)));
    scene.Add(make_shared<Sphere>(vec3(-1,0,-1), -0.45, make_shared<Dielectric>(1.5)));
}

void SceneLoad::InOneWeekLastScene(SceneObjects& scene) {
    //---------------- ray tracing in one weekend and the last scene
    auto checker_texture = make_shared<CheckerTexture>(color3(0.2, 0.3, 0.1), color3(0.9, 0.9, 0.9));
    scene.Add(make_shared<Sphere>(vec3(0, -1000, 0), 1000, make_shared<Lambertian>(checker_texture)));

    int sphere_number = 400;
    for(int i = 0; i < sphere_number; i ++) {
        auto generate_code = random_double();
        vec3 sphere_color(random_double(), random_double(), random_double());
        vec3 sphere_position(random_double(-5, 5), 0.2, random_double(-15, 3));
        if(generate_code < 0.7) {
            // diffuse
            scene.Add(make_shared<Sphere>(sphere_position, 0.2, make_shared<Lambertian>(make_shared<SolidColor>(sphere_color))));
        } else if(generate_code < 0.9) {
            // metal
            double fuzziness = random_double();
            scene.Add(make_shared<Sphere>(sphere_position, 0.2, make_shared<Metal>(make_shared<SolidColor>(sphere_color), fuzziness)));
        } else {
            // glass
            scene.Add(make_shared<Sphere>(sphere_position, 0.2, make_shared<Dielectric>(1.5)));
        }
    }

    scene.Add(make_shared<Sphere>(vec3(0, 1, -2), 1.0, make_shared<Dielectric>(1.5)));
    scene.Add(make_shared<Sphere>(vec3(0, 1, -4), 1.0, make_shared<Lambertian>(make_shared<SolidColor>(0.4, 0.2, 0.1))));
    scene.Add(make_shared<Sphere>(vec3(0, 1, 0), 1.0, make_shared<Metal>(make_shared<SolidColor>(0.7, 0.6, 0.5), 0.0)));
}

void SceneLoad::ChecherTextureTest(SceneObjects& scene) {
    //---------------- checher texture test
    auto checker_texture = make_shared<CheckerTexture>(color3(0.2, 0.3, 0.1), color3(0.9, 0.9, 0.9));
    scene.Add(make_shared<Sphere>(vec3(0, 10, 0), 10, make_shared<Lambertian>(checker_texture)));
    scene.Add(make_shared<Sphere>(vec3(0, -10, 0), 10, make_shared<Lambertian>(checker_texture)));
}

void SceneLoad::NoiseTextureTest(SceneObjects& scene) {
    //---------------- noise texture test
    auto pertext = make_shared<NoiseTexture>(1);
    scene.Add(make_shared<Sphere>(vec3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    scene.Add(make_shared<Sphere>(vec3(0, 2, 0), 2, make_shared<Lambertian>(pertext)));
}

void SceneLoad::ExternImageTextureTest(SceneObjects& scene) {
    //---------------- extern image texture test
    auto image_texture = make_shared<ImageTexture>(std::string("resource/earthmap.jpg"));
    auto earth_surface = make_shared<Lambertian>(image_texture);
    auto globe = make_shared<Sphere>(vec3(0, 0, -60), 60, earth_surface);
    scene.Add(globe);
}

void SceneLoad::LightTest(SceneObjects& scene) {
    //---------------- light test
    auto pertext = make_shared<NoiseTexture>(1);
    scene.Add(make_shared<Sphere>(vec3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    scene.Add(make_shared<Sphere>(vec3(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

    auto light_material = make_shared<DiffuseLight>(make_shared<SolidColor>(1, 1, 1));
    scene.Add(make_shared<XYRectangle>(-2, 2, 0, 3, -3, light_material));
    scene.Add(make_shared<YZRectangle>(0, 3, -2, 2, 3, light_material));
    scene.Add(make_shared<XZRectangle>(-2, 2, -2, 2, 4, light_material));
}

void SceneLoad::BoxTest(SceneObjects& scene) {
    //---------------- box test
    auto pertext = make_shared<NoiseTexture>(1);
    scene.Add(make_shared<Sphere>(vec3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    scene.Add(make_shared<Box>(point3(-2, 0, -2), point3(2, 2, 2), make_shared<Lambertian>(pertext)));

    auto light_material = make_shared<DiffuseLight>(make_shared<SolidColor>(1, 1, 1));
    scene.Add(make_shared<XYRectangle>(-2, 2, 0, 3, -3, light_material));
    scene.Add(make_shared<YZRectangle>(0, 3, -2, 2, 3, light_material));
    scene.Add(make_shared<XZRectangle>(-2, 2, -2, 2, 4, light_material));
}

void SceneLoad::CornellBoxTest(SceneObjects& scene) {
    // cornell box test
    auto red = make_shared<Lambertian>(make_shared<SolidColor>(0.65, 0.05, 0.05));
    auto white = make_shared<Lambertian>(make_shared<SolidColor>(0.73, 0.73, 0.73));
    auto green = make_shared<Lambertian>(make_shared<SolidColor>(0.12, 0.45, 0.15));
    auto light = make_shared<DiffuseLight>(make_shared<SolidColor>(60, 60, 60));  // light strength

    scene.Add(make_shared<YZRectangle>(0, 555, 0, 555, 0, green));
    scene.Add(make_shared<YZRectangle>(0, 555, 0, 555, 555, red));
    scene.Add(make_shared<XZRectangle>(213, 343, 227, 332, 554, light));
    scene.Add(make_shared<XZRectangle>(0, 555, 0, 555, 0, white));
    scene.Add(make_shared<XZRectangle>(0, 555, 0, 555, 555, white));
    scene.Add(make_shared<XYRectangle>(0, 555, 0, 555, 0, white));

    shared_ptr<Hittable> box1 = make_shared<Box>(point3(0, 0, 0), point3(150, 320, 150), white);
    box1 = make_shared<RotateY>(box1, 18);
    box1 = make_shared<Translate>(box1, vec3(130, 0, 65));
    scene.Add(box1);

    shared_ptr<Hittable> box2 = make_shared<Box>(point3(0, 0, 0), point3(150, 150, 150), white);
    box2 = make_shared<RotateY>(box2, -15);
    box2 = make_shared<Translate>(box2, vec3(290, 0, 225));
    scene.Add(box2);
}

} // namespace lxrt