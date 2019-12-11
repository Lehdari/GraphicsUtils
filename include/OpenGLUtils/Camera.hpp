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


namespace gut {

    class Camera {
    public:
        Camera(void);

        void lookAt(const Vec3f& eye, const Vec3f& target, const Vec3f& up);

        void projection(float fov, float aspectRatio, float near, float far);

        Mat4f getWorldToClip(void) const;

    private:
        Mat4f _worldToCamera;
        Mat4f _cameraToClip;
    };

} // namespace gut


#endif //GRAPHICSUTILS_CAMERA_HPP
