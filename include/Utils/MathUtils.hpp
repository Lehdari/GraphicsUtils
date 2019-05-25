//
// Created by Lehdari on 23.7.2018.
//

#ifndef INC_2DTEST_MATHUTILS_HPP
#define INC_2DTEST_MATHUTILS_HPP


#include "MathTypes.hpp"
#include "TypeUtils.hpp"

#include <random>
#include <ctime>


#define PI 3.14159265359


inline double cubicInterpolate (const double (&p)[4], double x)
{
    return p[1] + 0.5 * x*(p[2] - p[0] + x*(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*(3.0*(p[1] - p[2]) + p[3] - p[0])));
}

inline Vec2f cubicInterpolate(const Vec2f (&p)[4], double x)
{
    return p[1] + 0.5 * x*(p[2] - p[0] + x*(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*(3.0*(p[1] - p[2]) + p[3] - p[0])));
}

inline float rnd(float min = 0.0f, float max = 1.0f)
{
    static std::default_random_engine r(time(NULL));
    return min+(max-min)*(r()%65536)*0.0000152590219f;
}

void fitPlane(Vec3f& p, Vec3f& n, const Vector<Vec3f>& points);


#endif //INC_2DTEST_MATHUTILS_HPP
