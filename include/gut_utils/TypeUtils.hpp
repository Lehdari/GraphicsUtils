//
// Project: GraphicsUtils
// File: TypeUtils.hpp
//
// Copyright (c) 2019 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#ifndef GRAPHICSUTILS_TYPEUTILS_HPP
#define GRAPHICSUTILS_TYPEUTILS_HPP


#include "MathTypes.hpp"

#include <Eigen/StdVector>


#define GUT_ALIGNED_VECTOR_EIGEN(TYPE) template <>\
struct VectorType<TYPE> {\
    static std::vector<TYPE, Eigen::aligned_allocator<TYPE>> value;\
};


template <typename T>
struct VectorType {
    static std::vector<T> value;
};

// Specializations for vector types
GUT_ALIGNED_VECTOR_EIGEN(Vec2f);
GUT_ALIGNED_VECTOR_EIGEN(Vec3f);
GUT_ALIGNED_VECTOR_EIGEN(Vec4f);
GUT_ALIGNED_VECTOR_EIGEN(Mat2f);
GUT_ALIGNED_VECTOR_EIGEN(Mat3f);
GUT_ALIGNED_VECTOR_EIGEN(Mat4f);

GUT_ALIGNED_VECTOR_EIGEN(Vec2d);
GUT_ALIGNED_VECTOR_EIGEN(Vec3d);
GUT_ALIGNED_VECTOR_EIGEN(Vec4d);
GUT_ALIGNED_VECTOR_EIGEN(Mat2d);
GUT_ALIGNED_VECTOR_EIGEN(Mat3d);
GUT_ALIGNED_VECTOR_EIGEN(Mat4d);

GUT_ALIGNED_VECTOR_EIGEN(Vec2i);
GUT_ALIGNED_VECTOR_EIGEN(Vec3i);
GUT_ALIGNED_VECTOR_EIGEN(Vec4i);
GUT_ALIGNED_VECTOR_EIGEN(Mat2i);
GUT_ALIGNED_VECTOR_EIGEN(Mat3i);
GUT_ALIGNED_VECTOR_EIGEN(Mat4i);


template <typename T>
using Vector = decltype(VectorType<T>::value);


template <typename T>
using Vector2D = Vector<Vector<T>>;


#endif //GRAPHICSUTILS_TYPEUTILS_HPP
