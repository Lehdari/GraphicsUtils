//
// Created by Lehdari on 23.7.2018.
//

#ifndef INC_2DTEST_TYPEUTILS_HPP
#define INC_2DTEST_TYPEUTILS_HPP


#include "MathTypes.hpp"

#include <Eigen/StdVector>


template <typename T>
struct VectorType {
    static std::vector<T> value;
};

template <>
struct VectorType<Vec2f> {
    static std::vector<Vec2f, Eigen::aligned_allocator<Vec2f>> value;
};

template <>
struct VectorType<Vec3f> {
    static std::vector<Vec3f, Eigen::aligned_allocator<Vec3f>> value;
};

template <>
struct VectorType<Vec4f> {
    static std::vector<Vec4f, Eigen::aligned_allocator<Vec4f>> value;
};

template <>
struct VectorType<Mat2f> {
    static std::vector<Mat2f, Eigen::aligned_allocator<Mat2f>> value;
};

template <>
struct VectorType<Mat3f> {
    static std::vector<Mat3f, Eigen::aligned_allocator<Mat3f>> value;
};

template <>
struct VectorType<Mat4f> {
    static std::vector<Mat4f, Eigen::aligned_allocator<Mat4f>> value;
};


template <typename T>
using Vector = decltype(VectorType<T>::value);


template <typename T>
using Vector2D = Vector<Vector<T>>;


#endif //INC_2DTEST_TYPEUTILS_HPP
