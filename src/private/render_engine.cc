#include "render_engine.h"
#include "denoise.h"

namespace lxrt {

void RenderEngine::Render() {
    // init class
    if(is_render_use_) {
        render_return_ = RenderReturnType::RepeatUse;
        return;
    }
    is_render_use_ = true;

    std::string scene_message;
    auto create_scene = SceneLoadObject::get()->GetScene(scene_name_, scene_message);
    if(create_scene) {
        create_scene(scene_);
    } else {
        std::cout << "Don't find the scene!" << std::endl;
        render_return_ = RenderReturnType::SceneError;
        return;
    }

    // base setting
    Camera camera(render_data_.look_from, render_data_.look_at, render_data_.look_up, render_data_.fov, render_data_.aspect, render_data_.aperture, render_data_.focus_distance, 0, 1);

    // output image
    ShowImage show_image(render_data_.screen_width, render_data_.screen_height, render_data_.channel);
    show_image.is_open_gamma(true);

    // G-buffer -------------------
    ShowImage depth_image(render_data_.screen_width, render_data_.screen_height, render_data_.channel);
    ShowImage normal_image(render_data_.screen_width, render_data_.screen_height, render_data_.channel);

    // rendering
    #ifndef __linux__
    clock_t start_time = clock();
    #else
    struct timeval sTime, eTime;
    gettimeofday(&sTime, NULL);
    #endif
    
    ///////////////////////thread start
    #ifdef OPEN_THREAD
    int cpu_nums = GetSystemCpuNumbers();
    int thread_nums = cpu_nums * 2;
    int step = render_data_.screen_height / thread_nums;
    std::vector<std::unique_ptr<std::thread>> th(thread_nums);
    for(int i = 0; i < thread_nums; i ++) {
        int starti = step * i;
        int endi = i == thread_nums - 1 ? render_data_.screen_height : step * (i + 1);
        th[i] = std::make_unique<std::thread>(std::bind(&RenderEngine::ThreadFunction, this, scene_, camera, std::ref(show_image), starti, endi, std::ref(depth_image), std::ref(normal_image)));
    }
    for(auto& it: th) {
        it->detach();
    }

    int front_process = 0;
    std::cerr << "[>-------------------------------------------------]\r[>";
    while(thread_completed < render_data_.screen_height * render_data_.screen_width) {
        int now_process = float(thread_completed) / render_data_.screen_height / render_data_.screen_width * 50.0;
        if(now_process != front_process) {
            std::cerr << "\b";
            for(int k = 0; k < now_process - front_process; k ++) {
                std::cerr <<  "=";
            }
            std::cerr << ">";
            front_process = now_process;
        }
    }
    //////////////////////thread end
    #else
    //////////////////////no thread start
    int front_process = 0;
    std::cerr << "[>-------------------------------------------------]\r[>";
    for(int i = 0; i < screen_height; i ++) {
        int now_process = (float)i / screen_height * 50.0;
        if(now_process != front_process) {
            LSleep(1);
            std::cerr << "\b";
            for(int k = 0; k < now_process - front_process; k ++) {
                std::cerr <<  "=";
            }
            std::cerr << ">";
            front_process = now_process;
        }
        
        for(int j = 0; j < screen_width; j ++) {
            color3 color(0, 0, 0);
            for(int s = 0; s < samples_per_pixel; s ++) {
                auto u = (i + random_double()) / screen_height;
                auto v = (j + random_double()) / screen_width;
                color += RayColor(camera.get_ray(u, v), background, scene_, rendering_depth);
            }
            color /= samples_per_pixel;
            show_image.SetColorAlpha(i, j, color[0], color[1], color[2], 1.0);
        }
    }
    //////////////////////no thread end
    #endif
    
    #ifndef __linux__
    clock_t end_time = clock();
    double duration_time = end_time - start_time;
    #else
    gettimeofday(&eTime, NULL);
    long duration_time = (eTime.tv_sec-sTime.tv_sec)*1000000+(eTime.tv_usec-sTime.tv_usec);
    duration_time /= 1000;
    #endif
    std::cerr << std::endl << "Ending" << std::endl << "Total time is ";
    if(duration_time / 1000 >= 60) std::cerr << duration_time / 60000 << "min";
    std::cerr << duration_time / 1000 % 60 << "s" << std::endl;

    show_image.Show("output/output.png");
    show_image.WriteFileWithRealData("output/output_png.txt");
    depth_image.Show("output/depth_output.png");
    normal_image.Show("output/normal_output.png");

    render_return_ = RenderReturnType::RenderComplete;
}

color3 RenderEngine::RayColor(const Ray &ray, const vec3& background, const Hittable &objects, int depth) {
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

void RenderEngine::ThreadFunction(const SceneObjects& scene, const Camera& camera, ShowImage& show_image, int starti, int endi, ShowImage& depth_image, ShowImage& normal_image)
{
    for(int i = starti; i < endi; i ++) {
        for(int j = 0; j < render_data_.screen_width; j ++) {
            ++ thread_completed;
            color3 color(0, 0, 0);
            double depth = 0;
            vec3 normal(0);
            for(int s = 0; s < render_data_.samples_per_pixel; s ++) {
                auto u = (i + random_double()) / render_data_.screen_height;
                auto v = (j + random_double()) / render_data_.screen_width;
                color += RayColor(camera.get_ray(u, v), render_data_.background, scene, render_data_.rendering_depth);
                depth += GetDepth(camera.get_ray(u, v), scene);
                normal += GetNormal(camera.get_ray(u, v), scene);
            }
            color /= render_data_.samples_per_pixel;
            depth /= render_data_.samples_per_pixel;
            normal /= render_data_.samples_per_pixel;
            // std::cout << "(" << i << "," << j << "):  depth:" << depth << "  normal:" << normal << std::endl;
            show_image.SetColorAlpha(i, j, color[0], color[1], color[2], 1.0);
            depth_image.SetColorAlpha(i, j, depth / kSceneMaxDepth, depth / kSceneMaxDepth, depth / kSceneMaxDepth, 1.0);
            normal_image.SetColorAlpha(i, j, (normal.x() + 1.0) / 2, (normal.y() + 1.0) / 2, (normal.z() + 1.0) / 2, 1.0);
        }
    }
}

double RenderEngine::GetDepth(const Ray& ray, const Hittable& objects) {
    HitRecord hit_record;
    if(!objects.Hit(ray, 0.001, kInfinity, hit_record)) {
        return kSceneMaxDepth;
    }
    // return (hit_record.p - ray.origin()).length();  // perspective depth
    return (ray.origin().z() - hit_record.p.z());  // orthographic depth
}

vec3 RenderEngine::GetNormal(const Ray& ray, const Hittable& objects) {
    HitRecord hit_record;
    if(!objects.Hit(ray, 0.001, kInfinity, hit_record)) {
        return ray.direction().normalized();
    }
    return hit_record.normal;
}

void RenderEngine::LoadRenderData(RenderingParameters render_data) {
    render_data_ = render_data;
}

} // namespace lxrt