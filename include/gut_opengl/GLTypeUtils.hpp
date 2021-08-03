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
#include <gut_image/Image.hpp>


namespace gut {

    /** @brief  Get corresponding OpenGL enumeration for a data type
     *  @tparam T_Data  Data type to get corresponding enumeration for
     *  @return OpenGL enumeration corresponding to the data type
     */
    template <typename T_Data>
    constexpr GLenum typeToGLEnum();

    /** @brief  Get corresponding OpenGL enumeration for a image data format
     *  @param  dataFormat  Data format to get the corresponding enumeration for
     *  @return OpenGL enumeration corresponding to the image data format
     */
    GLenum imageDataFormatToGLEnum(Image::DataFormat dataFormat);

    /** @brief  Get corresponding OpenGL enumeration for a image data type
     *  @param  dataType    Data type to get the corresponding enumeration for
     *  @return OpenGL enumeration corresponding to the image data type
     */
    GLenum imageDataTypeToGLEnum(Image::DataType dataType);


    #include "GLTypeUtils.inl"

} // namespace gut


#endif //GRAPHICSUTILS_GLTYPEUTILS_HPP
