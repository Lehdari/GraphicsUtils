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


using namespace gut;


Texture::Texture(GLenum target, GLenum internalFormat) :
    _textureId      (0),
    _target         (target),
    _internalFormat (internalFormat),
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
    _textureId  (other._textureId)
{
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    _textureId = other._textureId;
    other._textureId = 0;

    return *this;
}

void Texture::create(int width, int height)
{
    create(width, height, _target, _internalFormat);
}

void Texture::create(int width, int height, GLenum target, GLenum internalFormat)
{
    _width = width;
    _height = height;
    _target = target;
    _internalFormat = internalFormat;

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
    glTexImage2D(_target, 0, _internalFormat, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glGenerateMipmap(_target);

    glBindTexture(_target, 0);
}

void Texture::create(int width, int height, int depth)
{
    create(width, height, depth, _target, _internalFormat);
}

void Texture::create(int width, int height, int depth, GLenum target, GLenum internalFormat)
{
    _width = width;
    _height = height;
    _depth = depth;
    _target = target;
    _internalFormat = internalFormat;

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
    glTexImage3D(_target, 0, _internalFormat, _width, _height, _depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glGenerateMipmap(_target);

    glBindTexture(_target, 0);
}

void Texture::loadFromFile(const std::string& fileName)
{
    loadFromFile(fileName, _target, _internalFormat);
}

void Texture::loadFromFile(const std::string& fileName, GLenum target, GLenum internalFormat)
{
    Image img;
    img.loadFromFile(fileName);
    loadFromImage(img, target, internalFormat);
}

void Texture::loadFromImage(const Image& image)
{
    loadFromImage(image, _target, _internalFormat);
}

void Texture::loadFromImage(const Image& image, GLenum target, GLenum internalFormat)
{
    _width = image.width();
    _height = image.height();

    _target = target;
    _internalFormat = internalFormat;

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
    // TODO support for floating point / 16bit image formats
    switch (image.dataFormat()) {
        case Image::DataFormat::RGB:
            glTexImage2D(_target, 0, _internalFormat, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data<uint8_t>());
            break;
        case Image::DataFormat::RGBA:
            glTexImage2D(_target, 0, _internalFormat, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data<uint8_t>());
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
        glBindImageTexture(unit, _textureId, level, GL_TRUE, 0, access, _internalFormat);
    else
        glBindImageTexture(unit, _textureId, level, GL_FALSE, 0, access, _internalFormat);
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
