//
// Project: GraphicsUtils
// File: MathTypes.hpp
//
// Copyright (c) 2019 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#ifndef GRAPHICSUTILS_MATHTYPES_HPP
#define GRAPHICSUTILS_MATHTYPES_HPP


#include <Eigen/Dense>
#include <cstdint>


using Vec2f = Eigen::Vector2f;
using Vec3f = Eigen::Vector3f;
using Vec4f = Eigen::Vector4f;
using Mat2f = Eigen::Matrix2f;
using Mat3f = Eigen::Matrix3f;
using Mat4f = Eigen::Matrix4f;

using Vec2d = Eigen::Vector2d;
using Vec3d = Eigen::Vector3d;
using Vec4d = Eigen::Vector4d;
using Mat2d = Eigen::Matrix2d;
using Mat3d = Eigen::Matrix3d;
using Mat4d = Eigen::Matrix4d;

using Vec2i = Eigen::Vector2i;
using Vec3i = Eigen::Vector3i;
using Vec4i = Eigen::Vector4i;
using Mat2i = Eigen::Matrix2i;
using Mat3i = Eigen::Matrix3i;
using Mat4i = Eigen::Matrix4i;


#endif //GRAPHICSUTILS_MATHTYPES_HPP
