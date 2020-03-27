//
// Project: GraphicsUtils
// File: FrameBuffer.cpp
//
// Copyright (c) 2020 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENCE which is distributed
// with this source code package.
//

#include <gut_opengl/FrameBuffer.hpp>

#include "FrameBuffer.hpp"


using namespace gut;


FrameBuffer::FrameBuffer() :
    _frameBufferId  (0)
{
}

FrameBuffer::~FrameBuffer()
{
    reset();
}

FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept :
    _frameBufferId  (other._frameBufferId)
{
    other._frameBufferId = 0;
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept
{
    _frameBufferId = other._frameBufferId;
    other._frameBufferId = 0;
    return *this;
}

void FrameBuffer::create()
{
    if (_frameBufferId == 0)
        glGenFramebuffers(1, &_frameBufferId);
}

void FrameBuffer::bind() const noexcept
{
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferId);
    glDrawBuffers(_attachments.size(), _attachments.data());
}

void FrameBuffer::attachTexture(const Texture& texture, GLenum attachment)
{
    if (texture._target != GL_TEXTURE_2D) {
        fprintf(stderr, "Only 2D textures can be bound to framebuffer\n");
        return;
    }

    for (auto& a : _attachments) {
        if (a == attachment) {
            fprintf(stderr, "Attachment with give ID already exists.\n");
            return;
        }
    }

    bind();
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture._textureId, 0);

    _attachments.push_back(attachment);
}

void FrameBuffer::reset()
{
    if (_frameBufferId != 0)
        glDeleteFramebuffers(1, &_frameBufferId);
}
