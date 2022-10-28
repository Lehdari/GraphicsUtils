//
// Project: GraphicsUtils
// File: MathTypeReflection.hpp
//
// Copyright (c) 2022 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#ifndef GRAPHICSUTILS_MATHTYPEREFLECTION_HPP
#define GRAPHICSUTILS_MATHTYPEREFLECTION_HPP


#define GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(TYPE, ENUM, GLTF_STRING) \
template <> struct MathTypeReflection<TYPE> {                                \
    static constexpr MathTypeEnum   typeEnum        {ENUM};                  \
    static constexpr const char     typeString[]    {#TYPE};                 \
    static constexpr const char     gltfString[]    {GLTF_STRING};           \
};


#include "MathTypes.hpp"


namespace gut {

    enum class MathTypeEnum {
        FLOAT,
        DOUBLE,
        VEC2F,
        VEC3F,
        VEC4F,
        MAT2F,
        MAT3F,
        MAT4F,
        VEC2D,
        VEC3D,
        VEC4D,
        MAT2D,
        MAT3D,
        MAT4D,
        VEC2I,
        VEC3I,
        VEC4I,
        MAT2I,
        MAT3I,
        MAT4I,
        QUATF,
        QUATD
    };

    template <typename T_Matrix>
    struct MathTypeReflection {
        static constexpr const char typeString[]    {""};
    };

    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(float, MathTypeEnum::FLOAT, "SCALAR");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(double, MathTypeEnum::DOUBLE, "");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Vec2f, MathTypeEnum::VEC2F, "VEC2");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Vec3f, MathTypeEnum::VEC3F, "VEC3");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Vec4f, MathTypeEnum::VEC4F, "VEC4");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Mat2f, MathTypeEnum::MAT2F, "MAT2");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Mat3f, MathTypeEnum::MAT3F, "MAT3");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Mat4f, MathTypeEnum::MAT4F, "MAT4");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Vec2d, MathTypeEnum::VEC2D, "");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Vec3d, MathTypeEnum::VEC3D, "");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Vec4d, MathTypeEnum::VEC4D, "");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Mat2d, MathTypeEnum::MAT2D, "");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Mat3d, MathTypeEnum::MAT3D, "");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Mat4d, MathTypeEnum::MAT4D, "");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Vec2i, MathTypeEnum::VEC2I, "");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Vec3i, MathTypeEnum::VEC3I, "");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Vec4i, MathTypeEnum::VEC4I, "");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Mat2i, MathTypeEnum::MAT2I, "");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Mat3i, MathTypeEnum::MAT3I, "");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Mat4i, MathTypeEnum::MAT4I, "");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Quatf, MathTypeEnum::QUATF, "");
    GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION(Quatd, MathTypeEnum::QUATD, "");

} // namespace gut


#undef GRAPHICSUTILS_MATHTYPEREFLECTION_REFLECTION

#endif //GRAPHICSUTILS_MATHTYPEREFLECTION_HPP
