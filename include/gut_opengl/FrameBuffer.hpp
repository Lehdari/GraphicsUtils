//
// Project: GraphicsUtils
// File: FrameBuffer.hpp
//
// Copyright (c) 2020 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENCE which is distributed
// with this source code package.
//

#ifndef GRAPHICSUTILS_FRAMEBUFFER_HPP
#define GRAPHICSUTILS_FRAMEBUFFER_HPP


#include <glad/glad.h>
#include <vector>

#include "Texture.hpp"


namespace gut {

    class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();

        FrameBuffer(const FrameBuffer& other) = delete;
        FrameBuffer(FrameBuffer&& other) noexcept;
        FrameBuffer& operator=(const FrameBuffer& other) = delete;
        FrameBuffer& operator=(FrameBuffer&& other) noexcept;

        void create();

        void useDepthRenderbuffer(int width = -1, int height = -1);

        void bind() const noexcept;

        void attachTexture(const Texture& texture, GLenum attachment);

        GLuint id() const noexcept;

    private:
        GLuint              _frameBufferId;
        GLuint              _depthRenderbufferId;
        std::vector<GLenum> _attachments;
        int                 _width;
        int                 _height;

        void reset();
    };

} // namespace gut


#endif //GRAPHICSUTILS_FRAMEBUFFER_HPP
