#pragma once
#include <iostream>
#include <cmath>
#include "core.h"

namespace lxrt {

class vec3 {
public:
    vec3(): e{0, 0, 0} { }
    vec3(double t): e{t, t, t} { }
    vec3(double t0, double t1, double t2): e{t0, t1, t2} { }

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3 &v) {
        e[0] += v[0];
        e[1] += v[1];
        e[2] += v[2];
        return *this;
    }

    vec3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t) {
        return *this *= 1/t;
    }

    double length() const {
        return std::sqrt(length2());
    }

    double length2() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    void normalize() {
        *this /= length();
    }

    vec3 normalized() const {
        double l = length();
        return vec3(e[0] / l, e[1] / l, e[2] / l);
    }

    inline static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    inline static vec3 random(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

private:
    double e[3];
};

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << '(' << v[0] << ',' << v[1] << ',' << v[2] << ')';
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return vec3(v[0] * t, v[1] * t, v[2] * t);
}

inline vec3 operator*(double t, const vec3& v) {
    return v * t;
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline vec3 operator/(const vec3& v, double t) {
    return v * (1/t);
}

inline double dot(const vec3& u, const vec3& v) {
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u[1] * v[2] - u[2] * v[1],
                u[2] * v[0] - u[0] * v[2],
                u[0] * v[1] - u[1] * v[0]);
}

static vec3 RandomInUnitSphere() {
    // while(true) {
    //     auto p = vec3::random(-1, 1);
    //     if(p.length2() >= 1.0) continue;
    //     return p;
    // }

    auto a = random_double(0, 2 * kPI);
    auto z = random_double(-1, 1);
    auto r = std::sqrt(1 - z * z);
    return vec3(r * std::cos(a), r * std::sin(a), z);
}

static vec3 RandomInUnitDisk() {
    while (true) {
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length2() >= 1) continue;
        return p;
    }
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = dot(-uv, n);
    vec3 r_out_parallel =  etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_perp = -sqrt(1.0 - r_out_parallel.length2()) * n;
    return r_out_parallel + r_out_perp;
}

using point3 = vec3;
using color3 = vec3;

} // namespace lxrt