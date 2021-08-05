//
// Project: evolution_simulator_2
// File: GLTypeUtils.cpp
//
// Copyright (c) 2021 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENCE which is distributed
// with this source code package.
//

#include "GLTypeUtils.hpp"


using namespace gut;


GLenum gut::imageDataFormatToGLEnum(Image::DataFormat dataFormat)
{
    switch (dataFormat) {
        case Image::DataFormat::GRAY:
            return GL_RED;
        case Image::DataFormat::RGB:
            return GL_RGB;
        case Image::DataFormat::RGBA:
            return GL_RGBA;
    }

    // should never be reached, maybe new data formats were added?
    throw std::runtime_error("ERROR: imageDataFormatToGLEnum(): image data format to GLEnum conversion failed");
}

GLenum gut::imageDataTypeToGLEnum(Image::DataType dataType)
{
    switch (dataType) {
        case Image::DataType::U8:
            return GL_UNSIGNED_BYTE;
        case Image::DataType::U16:
            return GL_UNSIGNED_SHORT;
        case Image::DataType::F32:
            return GL_FLOAT;
    }

    // should never be reached, maybe new data types were added?
    throw std::runtime_error("ERROR: imageDataTypeToGLEnum(): image data type to GLEnum conversion failed");
}

Image::DataType gut::glEnumToImageDataType(GLenum dataType)
{
    switch (dataType) {
        case GL_UNSIGNED_BYTE:
            return Image::DataType::U8;
        case GL_UNSIGNED_SHORT:
            return Image::DataType::U16;
        case GL_FLOAT:
            return Image::DataType::F32;
    }

    // should never be reached, maybe new data types were added?
    throw std::runtime_error("ERROR: glEnumToImageDataType(): GLEnum conversion to image data type failed");
}
