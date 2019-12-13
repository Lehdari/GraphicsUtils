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


Texture::Texture(void) :
    _textureId  (0)
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

void Texture::loadFromFile(const std::string& fileName, GLenum internalFormat)
{
    // Load image using STB image
    int w, h, nChannels;
    unsigned char *data = stbi_load(fileName.c_str(), &w, &h, &nChannels, 0);

    if (!data) {
        fprintf(stderr, "Failed to load image %s\n", fileName.c_str());
        return;
    }

    if (nChannels != 3 && nChannels != 4) {
        fprintf(stderr, "Invalid number of channels on image %s: %d\n", fileName.c_str(), nChannels);
        return;
    }

    // Release the used resources
    reset();

    // Generate new texture
    glGenTextures(1, &_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);

    // Set filtering and wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Transfer data to OpenGL and generate mipmaps
    switch (nChannels) {
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        case 4:
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        default:
            break;
    }
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Release stb resources
    stbi_image_free(data);
}

void Texture::bind(GLenum textureUnit) const
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, _textureId);
}

void Texture::reset(void)
{
    if (_textureId != 0)
        glDeleteTextures(1, &_textureId);
}
