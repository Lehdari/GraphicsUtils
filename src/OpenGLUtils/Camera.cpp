//
// Project: GraphicsUtils
// File: Camera.cpp
//
// Copyright (c) 2019 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "Camera.hpp"


using namespace gut;


Camera::Camera(void) :
    _worldToCamera(Mat4f::Identity()),
    _cameraToClip(Mat4f::Identity())
{}

void Camera::lookAt(const Vec3f& eye, const Vec3f& target, const Vec3f& up) {
    // Right handed coordinate system
    Vec3f zAxis = (eye - target).normalized();
    Vec3f xAxis = up.cross(zAxis).normalized();
    Vec3f yAxis = zAxis.cross(xAxis);

    _worldToCamera << xAxis[0], xAxis[1], xAxis[2], -xAxis.dot(eye),
        yAxis[0], yAxis[1], yAxis[2], -yAxis.dot(eye),
        zAxis[0], zAxis[1], zAxis[2], -zAxis.dot(eye),
        0.0f    , 0.0f    , 0.0f    , 1.0f           ;
}

void Camera::projection(float fov, float aspectRatio, float near, float far) {
    float r = tanf(fov / 2.0f) * near;
    float t = r / aspectRatio;

    _cameraToClip << near/r, 0.0f  , 0.0f                  , 0.0f                     ,
        0.0f  , near/t, 0.0f                  , 0.0f                     ,
        0.0f  , 0.0f  , -(far+near)/(far-near), -2.0f*far*near/(far-near),
        0.0f  , 0.0f  , -1.0f                 , 0.0f                     ;
}

Mat4f Camera::getWorldToClip(void) const {
    return _cameraToClip * _worldToCamera;
}
