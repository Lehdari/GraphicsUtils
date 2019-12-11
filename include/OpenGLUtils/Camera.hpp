//
// Project: GraphicsUtils
// File: Camera.hpp
//
// Copyright (c) 2019 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#ifndef GRAPHICSUTILS_CAMERA_HPP
#define GRAPHICSUTILS_CAMERA_HPP


#include "Utils/MathTypes.hpp"
#include "Utils/MathUtils.hpp"


namespace gut {

    class Camera {
    public:
        struct Settings {
            Vec3f pos;
            Vec3f target;
            Vec3f up;
            float fov;
            float near;
            float far;
            float aspectRatio;

            explicit Settings(
                const Vec3f& pos    = Vec3f(0.f, 2.0f, 5.0f),
                const Vec3f& target = Vec3f(0.f, 1.f, 0.f),
                const Vec3f& up     = Vec3f(0.f, 1.f, 0.f),
                float fov           = 60.f*PI/180.f,
                float near          = 0.1f,
                float far           = 100.f,
                float aspectRatio   = 16.0/9.0) :
                pos(pos),
                target(target),
                up(up),
                fov(fov),
                near(near),
                far(far),
                aspectRatio(aspectRatio)
            {}
        };

        Camera(void);
        Camera(const Settings& settings);

        void lookAt(const Vec3f& eye, const Vec3f& target, const Vec3f& up);

        void projection(float fov, float aspectRatio, float near, float far);

        Mat4f getWorldToClip(void) const;

    private:
        Mat4f _worldToCamera;
        Mat4f _cameraToClip;
    };

} // namespace gut


#endif //GRAPHICSUTILS_CAMERA_HPP
