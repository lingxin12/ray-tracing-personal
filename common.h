#pragma once
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <cmath>
#include <string>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <bitset>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <memory>
#include <functional>
#include <random>
#include <atomic>
#include <thread>

// define

// using 
using std::shared_ptr;
using std::make_shared;

// constants
const double kInfinity = std::numeric_limits<double>::infinity();
const double kPI = 3.1415926535897932385;

// utility functions
inline double ffmin(double a, double b) {
    return a <= b ? a : b;
}

inline double ffmax(double a, double b) {
    return a >= b ? a : b;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0, 1.0);
    static std::mt19937 generator;
    static std::function<double()> rand_generator = std::bind(distribution, generator);
    return rand_generator();
}

inline double random_double(double min, double max) {
    return random_double() * (max - min) + min;
}

inline double clamp(double x, double min, double max) {
    if(x < min) return min;
    if(x > max) return max;
    return x;
}

inline double schlick(double cosine, double ref_idx) {
    auto r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}

inline double degrees_to_radians(double degree) {
    return degree / 180.0 * kPI;
}

// common headers
#include "vec3.hpp"
#include "ray.hpp"