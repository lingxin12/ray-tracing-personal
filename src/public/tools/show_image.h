#pragma once
#include "common.h"

namespace lxrt { 

class ShowImage {
public:
    ShowImage(int width, int height, int channel):
            width_(width), height_(height), channel_(channel), gamma_(2.2), is_gamma_(false) {
        data_ = new unsigned char[width * height * channel];
        real_data_ = new double[width * height * channel];
    }

    ShowImage(const std::string& path, int& width, int& height, int& channel): gamma_(2.2), is_gamma_(false) {
        int iw, ih, in;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* idata = stbi_load(path.c_str(), &iw, &ih, &in, 0);
        width = width_ = iw;
        height = height_ = ih;
        channel = channel_ = in;
        data_ = new unsigned char[width_ * height_ * channel_];
        real_data_ = new double[width_ * height_ * channel_];
        for(int i = 0; i < height_; i ++) {
            for(int j = 0; j < width_; j ++) {
                data_[i * width_ * channel_ + j * channel_] = idata[i * width_ * channel_ + j * channel_];
                data_[i * width_ * channel_ + j * channel_ + 1] = idata[i * width_ * channel_ + j * channel_ + 1];
                data_[i * width_ * channel_ + j * channel_ + 2] = idata[i * width_ * channel_ + j * channel_ + 2];
                data_[i * width_ * channel_ + j * channel_ + 3] = idata[i * width_ * channel_ + j * channel_ + 3];

                real_data_[i * width_ * channel_ + j * channel_] = idata[i * width_ * channel_ + j * channel_];
                real_data_[i * width_ * channel_ + j * channel_ + 1] = idata[i * width_ * channel_ + j * channel_ + 1];
                real_data_[i * width_ * channel_ + j * channel_ + 2] = idata[i * width_ * channel_ + j * channel_ + 2];
                real_data_[i * width_ * channel_ + j * channel_ + 3] = idata[i * width_ * channel_ + j * channel_ + 3];
            }
        }
        stbi_image_free(idata);
    }

    ShowImage(const ShowImage& image): width_(image.width_), height_(image.height_), channel_(image.channel_), gamma_(image.gamma_), is_gamma_(image.is_gamma_) {
        data_ = new unsigned char[width_ * height_ * channel_];
        real_data_ = new double[width_ * height_ * channel_];
        for(int i = 0; i < height_; i ++) {
            for(int j = 0; j < width_; j ++) {
                data_[i * width_ * channel_ + j * channel_] = image.data_[i * width_ * channel_ + j * channel_];
                data_[i * width_ * channel_ + j * channel_ + 1] = image.data_[i * width_ * channel_ + j * channel_ + 1];
                data_[i * width_ * channel_ + j * channel_ + 2] = image.data_[i * width_ * channel_ + j * channel_ + 2];
                data_[i * width_ * channel_ + j * channel_ + 3] = image.data_[i * width_ * channel_ + j * channel_ + 3];

                real_data_[i * width_ * channel_ + j * channel_] = image.real_data_[i * width_ * channel_ + j * channel_];
                real_data_[i * width_ * channel_ + j * channel_ + 1] = image.real_data_[i * width_ * channel_ + j * channel_ + 1];
                real_data_[i * width_ * channel_ + j * channel_ + 2] = image.real_data_[i * width_ * channel_ + j * channel_ + 2];
                real_data_[i * width_ * channel_ + j * channel_ + 3] = image.real_data_[i * width_ * channel_ + j * channel_ + 3];
            }
        }
    }

    ShowImage& operator=(const ShowImage& image) {
        width_ = image.width_;
        height_ = image.height_;
        channel_ = image.channel_;
        gamma_ = image.gamma_;
        is_gamma_ = image.is_gamma_;
        for(int i = 0; i < height_; i ++) {
            for(int j = 0; j < width_; j ++) {
                data_[i * width_ * channel_ + j * channel_] = image.data_[i * width_ * channel_ + j * channel_];
                data_[i * width_ * channel_ + j * channel_ + 1] = image.data_[i * width_ * channel_ + j * channel_ + 1];
                data_[i * width_ * channel_ + j * channel_ + 2] = image.data_[i * width_ * channel_ + j * channel_ + 2];
                data_[i * width_ * channel_ + j * channel_ + 3] = image.data_[i * width_ * channel_ + j * channel_ + 3];

                real_data_[i * width_ * channel_ + j * channel_] = image.real_data_[i * width_ * channel_ + j * channel_];
                real_data_[i * width_ * channel_ + j * channel_ + 1] = image.real_data_[i * width_ * channel_ + j * channel_ + 1];
                real_data_[i * width_ * channel_ + j * channel_ + 2] = image.real_data_[i * width_ * channel_ + j * channel_ + 2];
                real_data_[i * width_ * channel_ + j * channel_ + 3] = image.real_data_[i * width_ * channel_ + j * channel_ + 3];
            }
        }
        return *this;
    }
    
    ~ShowImage() {
        delete[] data_;
        delete[] real_data_;
    }

    void SetColorAlpha(int i, int j, double red, double green, double blue, double alpha) {
        real_data_[i * width_ * channel_ + j * channel_] = red;
        real_data_[i * width_ * channel_ + j * channel_ + 1] = green;
        real_data_[i * width_ * channel_ + j * channel_ + 2] = blue;
        real_data_[i * width_ * channel_ + j * channel_ + 3] = alpha;
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

    std::tuple<double, double, double, double> GetColorAlpha(int i, int j) {
        return std::tuple<double, double, double, double>(
            real_data_[i * width_ * channel_ + j * channel_],
            real_data_[i * width_ * channel_ + j * channel_ + 1],
            real_data_[i * width_ * channel_ + j * channel_ + 2],
            real_data_[i * width_ * channel_ + j * channel_ + 3]
        );
    }

    void Show(const std::string& path) {
        stbi_flip_vertically_on_write(true);
        stbi_write_png(path.c_str(), width_, height_, channel_, data_, 0);
    }

    void ReadFileWithRealData(const std::string& path) {
        std::ifstream in(path.c_str());
        if(in.is_open()) {
            for(int i = 0; i < height_; i ++) {
                for(int j = 0; j < width_; j ++) {
                    in >> real_data_[i * width_ * channel_ + j * channel_];     in.ignore();
                    in >> real_data_[i * width_ * channel_ + j * channel_ + 1]; in.ignore();
                    in >> real_data_[i * width_ * channel_ + j * channel_ + 2]; in.ignore();
                    in >> real_data_[i * width_ * channel_ + j * channel_ + 3]; in.ignore();

                    double red = real_data_[i * width_ * channel_ + j * channel_];
                    double green = real_data_[i * width_ * channel_ + j * channel_ + 1];
                    double blue = real_data_[i * width_ * channel_ + j * channel_ + 2];
                    double alpha = real_data_[i * width_ * channel_ + j * channel_ + 3];
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
            }
        }
        in.close();
    }

    void WriteFileWithRealData(const std::string& path) {
        std::ofstream out(path.c_str());
        if(out.is_open()) {
            for(int i = 0; i < height_; i ++) {
                for(int j = 0; j < width_; j ++) {
                    out << real_data_[i * width_ * channel_ + j * channel_] << ' ';
                    out << real_data_[i * width_ * channel_ + j * channel_ + 1] << ' ';
                    out << real_data_[i * width_ * channel_ + j * channel_ + 2] << ' ';
                    out << real_data_[i * width_ * channel_ + j * channel_ + 3] << ' ';
                }
            }
        }
        out.close();
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
    double* real_data_;
};

} // namespace lxrt