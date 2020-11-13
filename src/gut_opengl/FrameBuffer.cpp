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
    _frameBufferId          (0),
    _depthRenderbufferId    (0),
    _width                  (-1),
    _height                 (-1)
{
}

FrameBuffer::~FrameBuffer()
{
    reset();
}

FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept :
    _frameBufferId          (other._frameBufferId),
    _depthRenderbufferId    (other._depthRenderbufferId),
    _width                  (other._width),
    _height                 (other._height)
{
    other._frameBufferId = 0;
    other._depthRenderbufferId = 0;
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept
{
    _frameBufferId = other._frameBufferId;
    _depthRenderbufferId = other._depthRenderbufferId;
    _width = other._width;
    _height = other._height;

    other._frameBufferId = 0;
    other._depthRenderbufferId = 0;
    other._width = -1;
    other._height = -1;

    return *this;
}

void FrameBuffer::create()
{
    if (_frameBufferId == 0)
        glGenFramebuffers(1, &_frameBufferId);
}

void FrameBuffer::useDepthRenderbuffer(int width, int height)
{
    bind();
    if (_depthRenderbufferId == 0) {
        if (width < 0 || height < 0) {
            if (_width < 0 || _height < 0) {
                fprintf(stderr, "FrameBuffer::useDepthRenderbuffer: Unable to deduce dimensions\n");
                return;
            }

            width = _width;
            height = _height;
        }

        glGenRenderbuffers(1, &_depthRenderbufferId);
        glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderbufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderbufferId);
    }
}

void FrameBuffer::bind() const noexcept
{
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferId);
    if (_attachments.size() > 0)
        glDrawBuffers(_attachments.size(), _attachments.data());
}

void FrameBuffer::attachTexture(const Texture& texture, GLenum attachment)
{
    if (texture._target != GL_TEXTURE_2D) {
        fprintf(stderr, "Only 2D textures can be bound to framebuffer\n");
        return;
    }

    if (_attachments.empty()) {
        _width = texture._width;
        _height = texture._height;
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

    if (attachment == GL_DEPTH_ATTACHMENT ||
        attachment == GL_STENCIL_ATTACHMENT ||
        attachment == GL_DEPTH_STENCIL_ATTACHMENT)
        return;

    _attachments.push_back(attachment);
}

GLuint FrameBuffer::id() const noexcept
{
    return _frameBufferId;
}

void FrameBuffer::reset()
{
    if (_frameBufferId != 0)
        glDeleteFramebuffers(1, &_frameBufferId);
    if (_depthRenderbufferId != 0)
        glDeleteRenderbuffers(1, &_depthRenderbufferId);
}
