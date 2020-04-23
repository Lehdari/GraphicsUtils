//
// Project: GraphicsUtils
// File: GLTypeUtils.hpp
//
// Copyright (c) 2020 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#ifndef GRAPHICSUTILS_GLTYPEUTILS_HPP
#define GRAPHICSUTILS_GLTYPEUTILS_HPP


#include <glad/glad.h>

#include <gut_utils/MathTypes.hpp>


namespace gut {

/** @brief  Get corresponding GL enumeration for a data type
 */
    template <typename T_Data>
    constexpr GLenum typeToGLEnum();


    #include "GLTypeUtils.inl"


} // namespace gut


#endif //GRAPHICSUTILS_GLTYPEUTILS_HPP
