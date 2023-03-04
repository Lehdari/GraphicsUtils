//
// Project: GraphicsUtils
// File: GLTypeUtils.inl
//
// Copyright (c) 2020 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//


template <typename T_Data>
constexpr GLenum typeToGLEnum()
{
    //static_assert(false, "Error: GLenum requested for unknown type");
    return GL_FALSE;
}

// typeToGLEnum specializations
template<> constexpr GLenum typeToGLEnum<float>() { return GL_FLOAT; }
template<> constexpr GLenum typeToGLEnum<double>() { return GL_DOUBLE; }
template<> constexpr GLenum typeToGLEnum<int8_t>() { return GL_BYTE; }
template<> constexpr GLenum typeToGLEnum<uint8_t>() { return GL_UNSIGNED_BYTE; }
template<> constexpr GLenum typeToGLEnum<short>() { return GL_SHORT; }
template<> constexpr GLenum typeToGLEnum<unsigned short>() { return GL_UNSIGNED_SHORT; }
template<> constexpr GLenum typeToGLEnum<int>() { return GL_INT; }
template<> constexpr GLenum typeToGLEnum<unsigned int>() { return GL_UNSIGNED_INT; }
template<> constexpr GLenum typeToGLEnum<bool>() { return GL_BOOL; }
template<> constexpr GLenum typeToGLEnum<Vec2f>() { return GL_FLOAT_VEC2; }
template<> constexpr GLenum typeToGLEnum<Vec3f>() { return GL_FLOAT_VEC3; }
template<> constexpr GLenum typeToGLEnum<Vec4f>() { return GL_FLOAT_VEC4; }
template<> constexpr GLenum typeToGLEnum<Mat2f>() { return GL_FLOAT_MAT2; }
template<> constexpr GLenum typeToGLEnum<Mat3f>() { return GL_FLOAT_MAT3; }
template<> constexpr GLenum typeToGLEnum<Mat4f>() { return GL_FLOAT_MAT4; }
template<> constexpr GLenum typeToGLEnum<Vec2d>() { return GL_DOUBLE_VEC2; }
template<> constexpr GLenum typeToGLEnum<Vec3d>() { return GL_DOUBLE_VEC3; }
template<> constexpr GLenum typeToGLEnum<Vec4d>() { return GL_DOUBLE_VEC4; }
template<> constexpr GLenum typeToGLEnum<Mat2d>() { return GL_DOUBLE_MAT2; }
template<> constexpr GLenum typeToGLEnum<Mat3d>() { return GL_DOUBLE_MAT3; }
template<> constexpr GLenum typeToGLEnum<Mat4d>() { return GL_DOUBLE_MAT4; }
