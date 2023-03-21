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
#include "lxrtgui.h"
#include "render_engine.h"

using namespace lxrt;

// whether open multithreaded
#define OPEN_THREAD

// screen size
int screen_width = 400;
int screen_height = 400;
const vec3 background(0, 0, 0);

// camera parameter
const double fov = 40;
const double aspect = double(screen_width / screen_height);
const vec3 look_from(278, 278, 800);
const vec3 look_at(278, 278, 0);
const vec3 look_up(0, 1, 0);
const double aperture = 0;
const double focus_distance = 2;

// image parameter
const int channel = 4;

// rendering parameter
int samples_per_pixel = 20;
int rendering_depth = 10;

std::atomic_int thread_completed(0);

color3 RayColor(const Ray &ray, const vec3& background, const Hittable &objects, int depth) {
    if(depth <= 0) {
        return vec3(0, 0, 0);
    }

    // HitRecord hit_record;
    // if(objects.Hit(ray, 0.001, kInfinity, hit_record)) {
    //     Ray scattered;
    //     vec3 attenuation;
    //     if(hit_record.material->scatter(ray, hit_record, attenuation, scattered)) {
    //         return attenuation * RayColor(scattered, background, objects, depth - 1);
    //     }
    //     return vec3(0, 0, 0);
    // }
    // vec3 unit_dir = ray.direction().normalized();
    // auto t = 0.5 * (unit_dir[1] + 1.0);
    // return (1.0 - t) * color3(1.0, 1.0, 1.0) + t * color3(0.5, 0.7, 1.0);

    HitRecord hit_record;
    if(!objects.Hit(ray, 0.001, kInfinity, hit_record)) {
        return background;
    }

    Ray scattered;
    vec3 attenuation;
    vec3 emitted = hit_record.material->emitted(hit_record.u, hit_record.v, hit_record.p);
    if(!hit_record.material->scatter(ray, hit_record, attenuation, scattered)) {
        return emitted;
    }

    return emitted + attenuation * RayColor(scattered, background, objects, depth - 1);
}

void CreateScene(SceneObjects& scene) {
    // cornell box test
    auto red = make_shared<Lambertian>(make_shared<SolidColor>(0.65, 0.05, 0.05));
    auto white = make_shared<Lambertian>(make_shared<SolidColor>(0.73, 0.73, 0.73));
    auto green = make_shared<Lambertian>(make_shared<SolidColor>(0.12, 0.45, 0.15));
    auto light = make_shared<DiffuseLight>(make_shared<SolidColor>(15, 15, 15));

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


    //---------------- box test
    // auto pertext = make_shared<NoiseTexture>(1);
    // scene.Add(make_shared<Sphere>(vec3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    // scene.Add(make_shared<Box>(point3(-2, 0, -2), point3(2, 2, 2), make_shared<Lambertian>(pertext)));

    // auto light_material = make_shared<DiffuseLight>(make_shared<SolidColor>(1, 1, 1));
    // scene.Add(make_shared<XYRectangle>(-2, 2, 0, 3, -3, light_material));
    // scene.Add(make_shared<YZRectangle>(0, 3, -2, 2, 3, light_material));
    // scene.Add(make_shared<XZRectangle>(-2, 2, -2, 2, 4, light_material));

    //---------------- light test
    // auto pertext = make_shared<NoiseTexture>(1);
    // scene.Add(make_shared<Sphere>(vec3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    // scene.Add(make_shared<Sphere>(vec3(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

    // auto light_material = make_shared<DiffuseLight>(make_shared<SolidColor>(1, 1, 1));
    // scene.Add(make_shared<XYRectangle>(-2, 2, 0, 3, -3, light_material));
    // scene.Add(make_shared<YZRectangle>(0, 3, -2, 2, 3, light_material));
    // scene.Add(make_shared<XZRectangle>(-2, 2, -2, 2, 4, light_material));

    //---------------- extern image texture test
    // auto image_texture = make_shared<ImageTexture>(std::string("resource/earthmap.jpg"));
    // auto earth_surface = make_shared<Lambertian>(image_texture);
    // auto globe = make_shared<Sphere>(vec3(0, 0, -60), 60, earth_surface);
    // scene.Add(globe);

    //---------------- noise texture test
    // auto pertext = make_shared<NoiseTexture>(1);
    // scene.Add(make_shared<Sphere>(vec3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    // scene.Add(make_shared<Sphere>(vec3(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

    //---------------- checher texture test
    // auto checker_texture = make_shared<CheckerTexture>(color3(0.2, 0.3, 0.1), color3(0.9, 0.9, 0.9));
    // scene.Add(make_shared<Sphere>(vec3(0, 10, 0), 10, make_shared<Lambertian>(checker_texture)));
    // scene.Add(make_shared<Sphere>(vec3(0, -10, 0), 10, make_shared<Lambertian>(checker_texture)));

    //---------------- ray tracing in one weekend
    // auto checker_texture = make_shared<CheckerTexture>(color3(0.2, 0.3, 0.1), color3(0.9, 0.9, 0.9));
    // scene.Add(make_shared<Sphere>(vec3(0, -1000, 0), 1000, make_shared<Lambertian>(checker_texture)));

    // int sphere_number = 400;
    // for(int i = 0; i < sphere_number; i ++) {
    //     auto generate_code = random_double();
    //     vec3 sphere_color(random_double(), random_double(), random_double());
    //     vec3 sphere_position(random_double(-5, 5), 0.2, random_double(-15, 3));
    //     if(generate_code < 0.7) {
    //         // diffuse
    //         scene.Add(make_shared<Sphere>(sphere_position, 0.2, make_shared<Lambertian>(make_shared<SolidColor>(sphere_color))));
    //     } else if(generate_code < 0.9) {
    //         // metal
    //         double fuzziness = random_double();
    //         scene.Add(make_shared<Sphere>(sphere_position, 0.2, make_shared<Metal>(make_shared<SolidColor>(sphere_color), fuzziness)));
    //     } else {
    //         // glass
    //         scene.Add(make_shared<Sphere>(sphere_position, 0.2, make_shared<Dielectric>(1.5)));
    //     }
    // }

    // scene.Add(make_shared<Sphere>(vec3(0, 1, -2), 1.0, make_shared<Dielectric>(1.5)));
    // scene.Add(make_shared<Sphere>(vec3(0, 1, -4), 1.0, make_shared<Lambertian>(make_shared<SolidColor>(0.4, 0.2, 0.1))));
    // scene.Add(make_shared<Sphere>(vec3(0, 1, 0), 1.0, make_shared<Metal>(make_shared<SolidColor>(0.7, 0.6, 0.5), 0.0)));
    
    // // Add BVH
    scene = SceneObjects(make_shared<BVHNode>(scene, 0, 0));
}

// void ThreadFunction(const SceneObjects &scene, const Camera& camera, ShowImage& show_image, int i)
void ThreadFunction(const SceneObjects& scene, const Camera& camera, ShowImage& show_image, int starti, int endi)
{
    for(int i = starti; i < endi; i ++) {
        for(int j = 0; j < screen_width; j ++) {
            ++ thread_completed;
            color3 color(0, 0, 0);
            for(int s = 0; s < samples_per_pixel; s ++) {
                auto u = (i + random_double()) / screen_height;
                auto v = (j + random_double()) / screen_width;
                color += RayColor(camera.get_ray(u, v), background, scene, rendering_depth);
            }
            color /= samples_per_pixel;
            show_image.SetColorAlpha(i, j, color[0], color[1], color[2], 1.0);
        }
    }
}

int main(int argc, char *argv[])
{
    SceneLoadObject::get()->Load();

    LXRTGUI::Main(argc, argv, "./output/output.png");
    int n = 1;
    // RenderEngine render_engine;
    // RenderingParameters data;
    // data.Initialize();
    // data.look_from = vec3(0, 0, 2);
    // data.background = vec3(0.35, 0.35, 0.35);
    // if(n >= 1) {
    //     render_engine.LoadRenderData(data);
    //     // render_engine.Render();
    //     std::thread th(&RenderEngine::Render, &render_engine);
    //     th.detach();
    //     -- n;
    // }

    // while(render_engine.GetNowProcessing() < 100) {
    //     std::cout << render_engine.GetNowProcessing() << std::endl;
    // }

    // // scene objects
    // SceneObjects scene;
    // CreateScene(scene);
    // // scene.Add(make_shared<Sphere>(vec3(0,0,-1), 0.5, make_shared<Lambertian>(vec3(0.7, 0.3, 0.3))));
    // // scene.Add(make_shared<Sphere>(vec3(0,-100.5,-1), 100, make_shared<Lambertian>(vec3(0.8, 0.8, 0.0))));

    // // scene.Add(make_shared<Sphere>(vec3(1,0,-1), 0.5, make_shared<Metal>(vec3(0.8, 0.6, 0.2), 0.2)));
    // // scene.Add(make_shared<Sphere>(vec3(-1,0,-1), 0.5, make_shared<Dielectric>(1.5)));
    // // scene.Add(make_shared<Sphere>(vec3(-1,0,-1), -0.45, make_shared<Dielectric>(1.5)));

    // // base setting
    // Camera camera(look_from, look_at, look_up, fov, aspect, aperture, focus_distance, 0, 1);

    // ShowImage show_image(screen_width, screen_height, channel);
    // show_image.is_open_gamma(true);

    // // rendering
    // #ifndef __linux__
    // clock_t start_time = clock();
    // #else
    // struct timeval sTime, eTime;
    // gettimeofday(&sTime, NULL);
    // #endif

    // ///////////////////////thread start
    // #ifdef OPEN_THREAD
    // int cpu_nums = GetSystemCpuNumbers();
    // int thread_nums = cpu_nums * 2;
    // int step = screen_height / thread_nums;
    // std::vector<std::unique_ptr<std::thread>> th(thread_nums);
    // for(int i = 0; i < thread_nums; i ++) {
    //     int starti = step * i;
    //     int endi = i == thread_nums - 1 ? screen_height : step * (i + 1);
    //     th[i] = std::make_unique<std::thread>(ThreadFunction, scene, camera, std::ref(show_image), starti, endi);
    // }
    // for(auto& it: th) {
    //     it->detach();
    // }

    // int front_process = 0;
    // std::cerr << "[>-------------------------------------------------]\r[>";
    // while(thread_completed < screen_height * screen_width) {
    //     int now_process = float(thread_completed) / screen_height / screen_width * 50.0;
    //     if(now_process != front_process) {
    //         std::cerr << "\b";
    //         for(int k = 0; k < now_process - front_process; k ++) {
    //             std::cerr <<  "=";
    //         }
    //         std::cerr << ">";
    //         front_process = now_process;
    //     }
    // }
    // //////////////////////thread end
    // #else
    // //////////////////////no thread start
    // int front_process = 0;
    // std::cerr << "[>-------------------------------------------------]\r[>";
    // for(int i = 0; i < screen_height; i ++) {
    //     int now_process = (float)i / screen_height * 50.0;
    //     if(now_process != front_process) {
    //         LSleep(1);
    //         std::cerr << "\b";
    //         for(int k = 0; k < now_process - front_process; k ++) {
    //             std::cerr <<  "=";
    //         }
    //         std::cerr << ">";
    //         front_process = now_process;
    //     }
        
    //     for(int j = 0; j < screen_width; j ++) {
    //         color3 color(0, 0, 0);
    //         for(int s = 0; s < samples_per_pixel; s ++) {
    //             auto u = (i + random_double()) / screen_height;
    //             auto v = (j + random_double()) / screen_width;
    //             color += RayColor(camera.get_ray(u, v), background, scene, rendering_depth);
    //         }
    //         color /= samples_per_pixel;
    //         show_image.SetColorAlpha(i, j, color[0], color[1], color[2], 1.0);
    //     }
    // }
    // //////////////////////no thread end
    // #endif
    
    // #ifndef __linux__
    // clock_t end_time = clock();
    // double duration_time = end_time - start_time;
    // #else
    // gettimeofday(&eTime, NULL);
    // long duration_time = (eTime.tv_sec-sTime.tv_sec)*1000000+(eTime.tv_usec-sTime.tv_usec);
    // duration_time /= 1000;
    // #endif
    // std::cerr << std::endl << "Ending" << std::endl << "Total time is ";
    // if(duration_time / 1000 >= 60) std::cerr << duration_time / 60000 << "min";
    // std::cerr << duration_time / 1000 % 60 << "s" << std::endl;

    // show_image.Show("output/output.png");

    // return 0;
}