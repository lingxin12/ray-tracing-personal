#pragma once
#include "common.h"
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "include/stb_image_resize.h"

namespace lxrt {

class ShowImage {
public:
    explicit ShowImage(int width, int height, int channel):
            width_(width), height_(height), channel_(channel), gamma_(2.2), is_gamma_(false) {
        data_ = (unsigned char*)malloc(width * height * channel);
    }
    
    ~ShowImage() {
        delete[] data_;
    }

    void SetColorAlpha(int i, int j, double red, double green, double blue, double alpha) {
        if(is_gamma_) {
            red = pow(red, 1 / gamma_);
            green = pow(green, 1 / gamma_);
            blue = pow(blue, 1 / gamma_);
        }
        data_[i * width_ * channel_ + j * channel_] = int(clamp(red, 0, 1.0) * 255.99);
        data_[i * width_ * channel_ + j * channel_ + 1] = int(clamp(green, 0, 1.0) * 255.99);
        data_[i * width_ * channel_ + j * channel_ + 2] = int(clamp(blue, 0, 1.0) * 255.99);
        data_[i * width_ * channel_ + j * channel_ + 3] = int(clamp(alpha, 0, 1.0) * 255.99);
    }

    void Show(const std::string& path) {
        stbi_flip_vertically_on_write(true);
        stbi_write_png(path.c_str(), width_, height_, channel_, data_, 0);
    }

    void set_gamma(double g) {
        gamma_ = g;
    }

    void is_open_gamma(bool open) {
        is_gamma_ = open;
    }

private:
    int width_, height_, channel_;
    double gamma_;
    bool is_gamma_;
    unsigned char* data_;
};

} // namespace lxrt