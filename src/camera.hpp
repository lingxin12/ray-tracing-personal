#pragma once
#include "common.h"

namespace lxrt {

class Camera {
public:
    Camera(vec3 look_from, vec3 look_at, vec3 look_up, double fov, double aspect, double aperture, double focus_distance, double t0, double t1) {
        origin_ = look_from;
        lens_radius_ = aperture / 2;

        time_start_ = t0;
        time_end_ = t1;
        
        auto fov_theta = degrees_to_radians(fov);
        auto half_height = tan(fov_theta / 2);
        auto half_width = aspect * half_height;
        camera_z_ = (look_from - look_at).normalized();
        camera_x_ = cross(look_up, camera_z_).normalized();
        camera_y_ = cross(camera_z_, camera_x_);

        lower_left_corner_ = origin_
                             - half_width * focus_distance * camera_x_
                             - half_height * focus_distance * camera_y_
                             - camera_z_;

        horizontal_ = 2 * half_width * focus_distance * camera_x_;
        vertical_ = 2 * half_height * focus_distance * camera_y_;
    }

    Ray get_ray(double u, double v) const {
        vec3 rd = lens_radius_ * RandomInUnitDisk();
        vec3 offset = camera_x_ * rd.x() + camera_y_ * rd.y();

        return Ray(origin_ + offset, 
                   lower_left_corner_+ u * vertical_ + v * horizontal_ - origin_ - offset,
                   random_double(time_start_, time_end_));
    }

private:
    vec3 origin_;
    vec3 lower_left_corner_;
    vec3 horizontal_;
    vec3 vertical_;
    vec3 camera_x_, camera_y_, camera_z_;
    double lens_radius_;
    double time_start_, time_end_;
};

} // namespace lxrt