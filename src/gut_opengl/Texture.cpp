//
// Project: GraphicsUtils
// File: Texture.cpp
//
// Copyright (c) 2019 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "Texture.hpp"
#include <gut_image/Image.hpp>
#include <stdexcept>


using namespace gut;


Texture::Texture(GLenum target, GLenum channelFormat, GLenum dataType) :
    _textureId      (0),
    _target         (target),
    _channelFormat  (channelFormat),
    _dataType       (dataType),
    _width          (0),
    _height         (0),
    _depth          (0)
{
}

Texture::~Texture(void)
{
    reset();
}

Texture::Texture(Texture&& other) noexcept :
    _textureId      (other._textureId),
    _target         (other._target),
    _channelFormat  (other._channelFormat),
    _dataType       (other._dataType),
    _width          (other._width),
    _height         (other._height),
    _depth          (other._depth)
{
    other._textureId = 0;
    other._width = 0;
    other._height = 0;
    other._depth = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    _textureId = other._textureId;
    _target = other._target;
    _channelFormat = other._channelFormat;
    _dataType = other._dataType;
    _width = other._width;
    _height = other._height;
    _depth = other._depth;
    other._textureId = 0;
    other._width = 0;
    other._height = 0;
    other._depth = 0;

    return *this;
}

void Texture::create(int width, int height)
{
    create(width, height, _target, _channelFormat, _dataType);
}

void Texture::create(int width, int height, GLenum target, GLenum channelFormat, GLenum dataType)
{
    _width = width;
    _height = height;
    _target = target;
    _channelFormat = channelFormat;
    _dataType = dataType;

    // Release the used resources
    reset();

    // Generate new texture
    glGenTextures(1, &_textureId);
    glBindTexture(_target, _textureId);

    // Set filtering and wrapping
    glTexParameteri(_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = GL_RGBA;
    if (channelFormat == GL_DEPTH_COMPONENT ||
        channelFormat == GL_DEPTH_STENCIL)
        format = channelFormat;

    // Transfer data to OpenGL and generate mipmaps
    glTexImage2D(_target, 0, _channelFormat, _width, _height, 0, format, _dataType, nullptr);

    glGenerateMipmap(_target);

    glBindTexture(_target, 0);
}

void Texture::create(int width, int height, int depth)
{
    create(width, height, depth, _target, _channelFormat, _dataType);
}

void Texture::create(int width, int height, int depth,
    GLenum target, GLenum channelFormat, GLenum dataType)
{
    _width = width;
    _height = height;
    _depth = depth;
    _target = target;
    _channelFormat = channelFormat;
    _dataType = dataType;

    // Release the used resources
    reset();

    // Generate new texture
    glGenTextures(1, &_textureId);
    glBindTexture(_target, _textureId);

    // Set filtering and wrapping
    glTexParameteri(_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Transfer data to OpenGL and generate mipmaps
    glTexImage3D(_target, 0, _channelFormat, _width, _height, _depth, 0, GL_RGBA, _dataType, nullptr);

    glGenerateMipmap(_target);

    glBindTexture(_target, 0);
}

void Texture::loadFromFile(const std::string& fileName)
{
    loadFromFile(fileName, _target, _channelFormat);
}

void Texture::loadFromFile(const std::string& fileName, GLenum target, GLenum channelFormat)
{
    Image img;
    img.loadFromFile(fileName);
    loadFromImage(img, target, channelFormat);
}

void Texture::loadFromImage(const Image& image)
{
    loadFromImage(image, _target, _channelFormat);
}

void Texture::loadFromImage(const Image& image, GLenum target, GLenum channelFormat)
{
    _width = image.width();
    _height = image.height();

    _target = target;
    _channelFormat = channelFormat;

    switch (image.dataType()) {
        case Image::DataType::U8:
            _dataType = GL_UNSIGNED_BYTE;
            break;
        case Image::DataType::U16:
            _dataType = GL_UNSIGNED_SHORT;
            break;
        case Image::DataType::F32:
            _dataType = GL_FLOAT;
            break;
        default:
            throw std::runtime_error("ERROR: Texture::loadFromImage(): Invalid image data type");
    }

    // Release the used resources
    reset();

    // Generate new texture
    glGenTextures(1, &_textureId);
    glBindTexture(_target, _textureId);

    // Set filtering and wrapping
    glTexParameteri(_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Transfer data to OpenGL and generate mipmaps
    switch (image.dataFormat()) {
        case Image::DataFormat::RGB:
            switch (image.dataType()) {
                case Image::DataType::U8:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGB, _dataType, image.data<uint8_t>());
                    break;
                case Image::DataType::U16:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGB, _dataType, image.data<uint16_t>());
                    break;
                case Image::DataType::F32:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGB, _dataType, image.data<float>());
                    break;
                default:
                    throw std::runtime_error("ERROR: Texture::loadFromImage(): Invalid image data type");
            }
            break;
        case Image::DataFormat::RGBA:
            switch (image.dataType()) {
                case Image::DataType::U8:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGBA, _dataType, image.data<uint8_t>());
                    break;
                case Image::DataType::U16:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGBA, _dataType, image.data<uint16_t>());
                    break;
                case Image::DataType::F32:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGBA, _dataType, image.data<float>());
                    break;
                default:
                    throw std::runtime_error("ERROR: Texture::loadFromImage(): Invalid image data type");
            }
            break;
        default:
            break;
    }
    glGenerateMipmap(_target);

    glBindTexture(_target, 0);
}

void Texture::updateFromImage(const Image& image)
{
    _width = image.width();
    _height = image.height();

    switch (image.dataType()) {
        case Image::DataType::U8:
            _dataType = GL_UNSIGNED_BYTE;
            break;
        case Image::DataType::U16:
            _dataType = GL_UNSIGNED_SHORT;
            break;
        case Image::DataType::F32:
            _dataType = GL_FLOAT;
            break;
        default:
            throw std::runtime_error("ERROR: Texture::loadFromImage(): Invalid image data type");
    }

    glBindTexture(_target, _textureId);

    // Transfer data to OpenGL and generate mipmaps
    switch (image.dataFormat()) {
        case Image::DataFormat::RGB:
            switch (image.dataType()) {
                case Image::DataType::U8:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGB, _dataType, image.data<uint8_t>());
                    break;
                case Image::DataType::U16:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGB, _dataType, image.data<uint16_t>());
                    break;
                case Image::DataType::F32:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGB, _dataType, image.data<float>());
                    break;
                default:
                    throw std::runtime_error("ERROR: Texture::loadFromImage(): Invalid image data type");
            }
            break;
        case Image::DataFormat::RGBA:
            switch (image.dataType()) {
                case Image::DataType::U8:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGBA, _dataType, image.data<uint8_t>());
                    break;
                case Image::DataType::U16:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGBA, _dataType, image.data<uint16_t>());
                    break;
                case Image::DataType::F32:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGBA, _dataType, image.data<float>());
                    break;
                default:
                    throw std::runtime_error("ERROR: Texture::loadFromImage(): Invalid image data type");
            }
            break;
        default:
            break;
    }
    glGenerateMipmap(_target);

    glBindTexture(_target, 0);
}

void Texture::setFiltering(GLenum minFilter, GLenum magFilter)
{
    glBindTexture(_target, _textureId);

    // Set filtering
    glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Texture::setWrapping(GLenum sWrap, GLenum tWrap)
{
    glBindTexture(_target, _textureId);

    // Set filtering
    glTexParameteri(_target, GL_TEXTURE_WRAP_S, sWrap);
    glTexParameteri(_target, GL_TEXTURE_WRAP_T, tWrap);
}

void Texture::bind(GLenum textureUnit) const
{
    glActiveTexture(textureUnit);
    glBindTexture(_target, _textureId);
}

void Texture::bindImage(GLuint unit, GLint level, GLenum access) const
{
    if (_depth > 0)
        glBindImageTexture(unit, _textureId, level, GL_TRUE, 0, access, _channelFormat);
    else
        glBindImageTexture(unit, _textureId, level, GL_FALSE, 0, access, _channelFormat);
}

int Texture::width() const
{
    return _width;
}

int Texture::height() const
{
    return _height;
}

int Texture::depth() const
{
    return _depth;
}

GLuint Texture::id() const noexcept
{
    return _textureId;
}

void Texture::reset(void)
{
    if (_textureId != 0)
        glDeleteTextures(1, &_textureId);
}
