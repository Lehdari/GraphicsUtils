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
        /** @brief  Camera settings struct for initializing a Camera object
         */
        struct Settings {
            Vec3f pos;          ///< Camera origin
            Vec3f target;       ///< Point to look at
            Vec3f up;           ///< Upwards pointing vector in camera coordinates
            float fov;          ///< Horizontal field of view (in radians)
            float near;         ///< Distance of near-plane of view frustum from origin
            float far;          ///< Distance of far-plane of view frustum from origin
            float aspectRatio;  ///< Aspect ratio of the frustum (x/y)

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

        /** @Brief Orient the camera using look-at-point method
         *  @param eye      Camera origin position
         *  @param target   Point to look at
         *  @param up       Upwards pointing vector in camera coordinates
         */
        void lookAt(const Vec3f& eye, const Vec3f& target, const Vec3f& up);

        /** @brief Set the camera projection
         *  @param fov          Horizontal field of view (in radians)
         *  @param aspectRatio  Aspect ratio of the frustum (x/y)
         *  @param near         Distance of near-plane of view frustum from origin
         *  @param far          Distance of far-plane of view frustum from origin
         */
        void projection(float fov, float aspectRatio, float near, float far);

        /** @brief  Set world-to-camera matrix (camera transformation matrix) manually
         *  @param  worldToCamera   World-to-camera matrix
         */
        void setWorldToCamera(const Mat4f& worldToCamera);

        /** @brief  Get world-to-camera matrix (camera transformation matrix)
         *  @return World-to-camera matrix
         */
        const Mat4f& worldToCamera() const noexcept;

        /** @brief  Get camera-to-clip matrix (camera projection matrix)
         *  @return Camera-to-clip matrix
         */
        const Mat4f& cameraToClip() const noexcept;

        /** @brief  Get world-to-clip matrix (camera-to-clip matrix and world-to-camera matrix multiplied)
         *  @return World-to-clip matrix
         */
        Mat4f worldToClip() const noexcept;

    private:
        Mat4f _worldToCamera;
        Mat4f _cameraToClip;
    };

} // namespace gut


#endif //GRAPHICSUTILS_CAMERA_HPP
