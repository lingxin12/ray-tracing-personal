#include "denoise.h"

namespace lxrt {

void Denoise::Run(const std::string& input_image_path,
                  const std::string& depth_image_path,
                  const std::string& normal_image_path,
                  const std::string& output_image_path) {
    int iw, ih, in;
    ShowImage depth_image(depth_image_path, iw, ih, in);
    ShowImage normal_image(normal_image_path, iw, ih, in);
    ShowImage image(iw, ih, in);
    image.is_open_gamma(true);
    image.ReadFileWithRealData(input_image_path);

    Denoising(image, depth_image, normal_image, iw, ih, in);

    image.Show(output_image_path);
}

double Denoise::Weight(double pos, double sigma_distance,
                       color3 color, color3 middle_color, double sigma_color,
                       double depth, double middle_depth, double sigma_depth,
                       vec3 normal, vec3 middle_normal, double sigma_normal,
                       OpenSigma& opens) {
    double dis_exp = std::pow(kE, (-pos) * pos / 2.0 / sigma_distance / sigma_distance);

    double color_exp = std::pow(kE, -((color - middle_color).length2() / 2.0 / sigma_color / sigma_color));
    
    // double depth_exp = std::pow(kE, -(depth - middle_depth) * (depth - middle_depth) / 2.0 / sigma_depth / sigma_depth);
    double depth_exp = (depth - middle_depth) <= 55.0 ?
                       sigma_depth :
                       std::pow(kE, -(depth - middle_depth) * (depth - middle_depth) / 2.0 / sigma_depth / sigma_depth);

    double normal_exp = std::pow(kE, -((normal - middle_normal).length2() / 2.0 / sigma_normal / sigma_normal * 16.0));

    double res = 1;
    res *= opens.pos ? dis_exp : 1.0;
    res *= opens.color ? color_exp : 1.0;
    res *= opens.depth ? depth_exp : 1.0;
    res *= opens.normal ? normal_exp : 1.0;

    return res;
}

void Denoise::Denoising(ShowImage& image, ShowImage& depth_image, ShowImage& normal_image, const int width, const int height, const int channel) {
    ShowImage temp = image;

    int radius = radius_;

    double sigma_distance = sigma_.pos;
    double sigma_color = sigma_.color;
    double sigma_depth = sigma_.depth;
    double sigma_normal = sigma_.normal;

    OpenSigma opens;
    opens.pos = opens_.pos;
    opens.color = opens_.color;
    opens.depth = opens_.depth;
    opens.normal = opens_.normal;

    // horizontal
    for(int i = 0; i < height; i ++) {
        for(int j = 0; j < width; j ++) {
            double tot_r = 0, tot_g = 0, tot_b = 0, tot_a = 0;
            double tot = 0;
            for(int k = -radius; k <= radius; k ++) {
                int x = j + k;
                if(j + k < 0 || j + k >= width) x = j - k;
                auto[r, g, b, a] = image.GetColorAlpha(i, x);
                auto[mr, mg, mb, ma] = image.GetColorAlpha(i, j);
                auto depth = std::get<0>(depth_image.GetColorAlpha(i, x)) * kSceneMaxDepth;
                auto middle_depth = std::get<0>(depth_image.GetColorAlpha(i, j)) * kSceneMaxDepth;
                vec3 normal(std::get<0>(normal_image.GetColorAlpha(i, x)), std::get<1>(normal_image.GetColorAlpha(i, x)), std::get<2>(normal_image.GetColorAlpha(i, x)));
                normal = (normal / 255.99) * 2.0 - vec3(1.0, 1.0, 1.0);
                vec3 middle_normal(std::get<0>(normal_image.GetColorAlpha(i, j)), std::get<1>(normal_image.GetColorAlpha(i, j)), std::get<2>(normal_image.GetColorAlpha(i, j)));
                middle_normal = (middle_normal / 255.99) * 2.0 - vec3(1.0, 1.0, 1.0);

                double gus = Weight(k, sigma_distance,
                                    color3(r, g, b), color3(mr, mg, mb), sigma_color,
                                    depth, middle_depth, sigma_depth,
                                    normal, middle_normal, sigma_normal,
                                    opens);
                
                tot_r += r * gus;
                tot_g += g * gus;
                tot_b += b * gus;
                tot += gus;
            }
            temp.SetColorAlpha(i, j, tot_r / tot, tot_g / tot, tot_b / tot, 1.0);
        }
    }

    // vertical
    for(int j = 0; j < width; j ++) {
        for(int i = 0; i < height; i ++) {
            double tot_r = 0, tot_g = 0, tot_b = 0, tot_a = 0;
            double tot = 0;
            for(int k = -radius; k <= radius; k ++) {
                int x = i + k;
                if(i + k < 0 || i + k >= height) x = i - k;
                auto[r, g, b, a] = temp.GetColorAlpha(x, j);
                auto[mr, mg, mb, ma] = image.GetColorAlpha(i, j);
                auto depth = std::get<0>(depth_image.GetColorAlpha(x, j)) * kSceneMaxDepth;
                auto middle_depth = std::get<0>(depth_image.GetColorAlpha(i, j)) * kSceneMaxDepth;
                vec3 normal(std::get<0>(normal_image.GetColorAlpha(x, j)), std::get<1>(normal_image.GetColorAlpha(x, j)), std::get<2>(normal_image.GetColorAlpha(x, j)));
                normal = (normal / 255.99) * 2.0 - vec3(1.0, 1.0, 1.0);
                vec3 middle_normal(std::get<0>(normal_image.GetColorAlpha(i, j)), std::get<1>(normal_image.GetColorAlpha(i, j)), std::get<2>(normal_image.GetColorAlpha(i, j)));
                middle_normal = (middle_normal / 255.99) * 2.0 - vec3(1.0, 1.0, 1.0);

                double gus = Weight(k, sigma_distance,
                                    color3(r, g, b), color3(mr, mg, mb), sigma_color,
                                    depth, middle_depth, sigma_depth,
                                    normal, middle_normal, sigma_normal,
                                    opens);

                tot_r += r * gus;
                tot_g += g * gus;
                tot_b += b * gus;
                tot += gus;
            }
            image.SetColorAlpha(i, j, tot_r / tot, tot_g / tot, tot_b / tot, 1.0);
        }
    }   
}

} // namespace lxrt