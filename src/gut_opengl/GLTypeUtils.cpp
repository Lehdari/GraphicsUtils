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

Image::DataFormat gut::glEnumToImageDataFormat(GLenum channelFormat)
{
    switch (channelFormat) {
        case GL_RED:
        case GL_R8:
        case GL_R8I:
        case GL_R8UI:
        case GL_R16:
        case GL_R16I:
        case GL_R16UI:
        case GL_R16F:
        case GL_R32I:
        case GL_R32UI:
        case GL_R32F:
            return Image::DataFormat::GRAY;
        case GL_RGB:
        case GL_RGB8:
        case GL_RGB8I:
        case GL_RGB8UI:
        case GL_RGB16:
        case GL_RGB16I:
        case GL_RGB16UI:
        case GL_RGB16F:
        case GL_RGB32I:
        case GL_RGB32UI:
        case GL_RGB32F:
            return Image::DataFormat::RGB;
        case GL_RGBA:
        case GL_RGBA2:
        case GL_RGBA4:
        case GL_RGBA12:
        case GL_RGBA8:
        case GL_RGBA8I:
        case GL_RGBA8UI:
        case GL_RGBA8_SNORM:
        case GL_RGBA16:
        case GL_RGBA16I:
        case GL_RGBA16UI:
        case GL_RGBA16F:
        case GL_RGBA16_SNORM:
        case GL_RGBA32I:
        case GL_RGBA32UI:
        case GL_RGBA32F:
            return Image::DataFormat::RGBA;
    }

    // should never be reached, maybe new channel formats were added?
    throw std::runtime_error("ERROR: glEnumToImageDataFormat(): Unknown GL channel format");
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

size_t gut::glDataTypeSize(GLenum dataType)
{
    switch (dataType) {
        case GL_UNSIGNED_BYTE:
            return 1;
        case GL_UNSIGNED_SHORT:
            return 2;
        case GL_FLOAT:
            return 4;
    }

    // should never be reached, maybe new data types were added?
    throw std::runtime_error("ERROR: glDataTypeSize(): Unknown GL data type");
}

int gut::glNumberOfChannels(GLenum channelFormat)
{
    switch (channelFormat) {
        case GL_RED:
        case GL_R8:
        case GL_R8I:
        case GL_R8UI:
        case GL_R16:
        case GL_R16I:
        case GL_R16UI:
        case GL_R16F:
        case GL_R32I:
        case GL_R32UI:
        case GL_R32F:
            return 1;
        case GL_RG:
        case GL_RG8:
        case GL_RG8I:
        case GL_RG8UI:
        case GL_RG16:
        case GL_RG16I:
        case GL_RG16UI:
        case GL_RG16F:
        case GL_RG32I:
        case GL_RG32UI:
        case GL_RG32F:
            return 2;
        case GL_RGB:
        case GL_RGB8:
        case GL_RGB8I:
        case GL_RGB8UI:
        case GL_RGB16:
        case GL_RGB16I:
        case GL_RGB16UI:
        case GL_RGB16F:
        case GL_RGB32I:
        case GL_RGB32UI:
        case GL_RGB32F:
            return 3;
        case GL_RGBA:
        case GL_RGBA2:
        case GL_RGBA4:
        case GL_RGBA12:
        case GL_RGBA8:
        case GL_RGBA8I:
        case GL_RGBA8UI:
        case GL_RGBA8_SNORM:
        case GL_RGBA16:
        case GL_RGBA16I:
        case GL_RGBA16UI:
        case GL_RGBA16F:
        case GL_RGBA16_SNORM:
        case GL_RGBA32I:
        case GL_RGBA32UI:
        case GL_RGBA32F:
            return 4;
    }

    // should never be reached, maybe new channel formats were added?
    throw std::runtime_error("ERROR: glNumberOfChannels(): Unknown GL channel format");
}
