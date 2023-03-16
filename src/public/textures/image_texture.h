#pragma once
#include "common.h"
#include "texture.h"

namespace lxrt {

class ImageTexture: public Texture {
public:
    ImageTexture() { }
    
    ImageTexture(unsigned char *pixels, int width, int height, int channel):
        data(pixels), width_(width), height_(height), channel_(channel) { }

    ImageTexture(const std::string& path) {
        data = stbi_load(path.c_str(), &width_, &height_, &channel_, 0);
        if(!data) {
            std::cerr << "cannot open image!\n";
        }
    }

    virtual color3 Value(double u, double v, const vec3& p) const override {
        if(data == nullptr) {
            return vec3(0, 1, 1);
        }

        auto i = static_cast<int>((  u) * width_);
        auto j = static_cast<int>((1-v) * height_ - 0.001);

        if(i < 0) i = 0;
        if(j < 0) j = 0;
        if(i > width_ - 1) i = width_ - 1;
        if(j > height_ - 1) j = height_ - 1;

        auto r = static_cast<int>(data[3*i + 3*width_*j+0]) / 255.0;
        auto g = static_cast<int>(data[3*i + 3*width_*j+1]) / 255.0;
        auto b = static_cast<int>(data[3*i + 3*width_*j+2]) / 255.0;

        return vec3(r, g, b);
    }

private:
    unsigned char *data;
    int width_, height_, channel_;
};

} // namespace lxrt