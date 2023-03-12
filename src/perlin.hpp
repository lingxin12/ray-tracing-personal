#pragma once
#include "common.h"

namespace lxrt {

class Perlin {
public:
    Perlin() {
        ranvec_ = new vec3[point_count_];

        for (int i = 0; i < point_count_; ++ i) {
            ranvec_[i] = vec3::random(-1, 1).normalized();
        }

        perm_x_ = perlin_generate_perm();
        perm_y_ = perlin_generate_perm();
        perm_z_ = perlin_generate_perm();
    }

    ~Perlin() {
        delete[] ranvec_;
        delete[] perm_x_;
        delete[] perm_y_;
        delete[] perm_z_;
    }

    double Noise(const vec3& p) const {
        auto u = p.x() - floor(p.x());
        auto v = p.y() - floor(p.y());
        auto w = p.z() - floor(p.z());
        int i = floor(p.x());
        int j = floor(p.y());
        int k = floor(p.z());
        vec3 c[2][2][2];

        for (int di=0; di < 2; di++)
            for (int dj=0; dj < 2; dj++)
                for (int dk=0; dk < 2; dk++)
                    c[di][dj][dk] = ranvec_[
                        perm_x_[(i+di) & 255] ^
                        perm_y_[(j+dj) & 255] ^
                        perm_z_[(k+dk) & 255]
                    ];

        return perlin_interp(c, u, v, w);
    }

    double Turb(const vec3& p, int depth=7) const {
        auto accum = 0.0;
        vec3 temp_p = p;
        auto weight = 1.0;

        for (int i = 0; i < depth; i++) {
            accum += weight * Noise(temp_p);
            weight *= 0.5;
            temp_p *= 2;
        }

        return fabs(accum);
    }

private:
    static const int point_count_ = 256;
    vec3* ranvec_;
    int* perm_x_;
    int* perm_y_;
    int* perm_z_;

    static int* perlin_generate_perm() {
        auto p = new int[point_count_];

        for (int i = 0; i < Perlin::point_count_; i++)
            p[i] = i;

        permute(p, point_count_);

        return p;
    }

    static void permute(int* p, int n) {
        for (int i = n-1; i > 0; i--) {
            int target = random_int(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    static double perlin_interp(vec3 c[2][2][2], double u, double v, double w) {
        auto uu = u*u*(3-2*u);
        auto vv = v*v*(3-2*v);
        auto ww = w*w*(3-2*w);
        auto accum = 0.0;

        for (int i=0; i < 2; i++)
            for (int j=0; j < 2; j++)
                for (int k=0; k < 2; k++) {
                    vec3 weight_v(u-i, v-j, w-k);
                    accum += (i*uu + (1-i)*(1-uu))
                            * (j*vv + (1-j)*(1-vv))
                            * (k*ww + (1-k)*(1-ww))
                            * dot(c[i][j][k], weight_v);
                }

        return accum;
    }
};

} // namespace lxrt