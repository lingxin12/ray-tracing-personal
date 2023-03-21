// common headers
#pragma once
#include "core.h"
#include "vec3.h"
#include "ray.h"

namespace lxrt {

struct RenderingParameters {
// screen size
int screen_width;
int screen_height;
vec3 background;

// camera parameter
double fov;
double aspect;
vec3 look_from;
vec3 look_at;
vec3 look_up;
double aperture;
double focus_distance;

// image parameter
int channel;

// rendering parameter
int samples_per_pixel;
int rendering_depth;

void Initialize() {
    screen_width = 400;
    screen_height = 400;
    background = vec3(0, 0, 0);

    fov = 20;
    aspect = double(screen_width / screen_height);
    look_from = vec3(0, 0, 1);
    look_at = vec3(0, 0, 0);
    look_up = vec3(0, 1, 0);
    aperture = 0;
    focus_distance = 2;

    channel = 4;

    samples_per_pixel = 20;
    rendering_depth = 10;
}

};

} // namespace lxrt