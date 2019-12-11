//
// Created by lehdari on 25.5.2019.
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
