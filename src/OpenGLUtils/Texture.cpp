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
#include <stb_image.h>


using namespace gut;


Texture::Texture(GLenum target, GLenum internalFormat) :
    _textureId      (0),
    _target         (target),
    _internalFormat (internalFormat)
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

void Texture::loadFromFile(const std::string& fileName)
{
    loadFromFile(fileName, _target, _internalFormat);
}

void Texture::loadFromFile(const std::string& fileName, GLenum target, GLenum internalFormat)
{
    // Load image using STB image
    int nChannels;
    unsigned char *data = stbi_load(fileName.c_str(), &_width, &_height, &nChannels, 0);

    if (!data) {
        fprintf(stderr, "Failed to load image %s\n", fileName.c_str());
        return;
    }

    if (nChannels != 3 && nChannels != 4) {
        fprintf(stderr, "Invalid number of channels on image %s: %d\n", fileName.c_str(), nChannels);
        return;
    }

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
    switch (nChannels) {
        case 3:
            glTexImage2D(_target, 0, _internalFormat, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        case 4:
            glTexImage2D(_target, 0, _internalFormat, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        default:
            break;
    }
    glGenerateMipmap(_target);

    glBindTexture(_target, 0);

    // Release stb resources
    stbi_image_free(data);
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

void Texture::reset(void)
{
    if (_textureId != 0)
        glDeleteTextures(1, &_textureId);
}
