#pragma once
#include "common.h"
#include "show_image.h"

namespace lxrt {

class Denoise {
public:
    struct OpenSigma {
        bool pos;
        bool color;
        bool depth;
        bool normal;
    };

    struct Sigma {
        double pos;
        double color;
        double depth;
        double normal;
    };

    Denoise(): radius_(2), opens_({false, false, false, false}), sigma_({2, 2, 2, 2}) { }

    Denoise(int radius,
            bool open_distance, bool open_color, bool open_depth, bool open_normal,
            double sigma_distance, double sigma_color, double sigma_depth, double sigma_normal):
            radius_(radius),
            opens_({open_distance, open_color, open_depth, open_normal}),
            sigma_({sigma_distance, sigma_color, sigma_depth, sigma_normal}) { }

    void Run(const std::string& input_image_path,
             const std::string& depth_image_path,
             const std::string& normal_image_path,
             const std::string& output_image_path);

private:
    double Weight(double pos, double sigma_distance,
                  color3 color, color3 middle_color, double sigma_color,
                  double depth, double middle_depth, double sigma_depth,
                  vec3 normal, vec3 middle_normal, double sigma_normal,
                  OpenSigma& opens);

    void Denoising(ShowImage& image, ShowImage& depth_image, ShowImage& normal_image, const int width, const int height, const int channel);

private:
    int radius_;
    OpenSigma opens_;
    Sigma sigma_;
};

} // namespace lxrt