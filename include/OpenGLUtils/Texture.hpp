//
// Project: GraphicsUtils
// File: Texture.hpp
//
// Copyright (c) 2019 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#ifndef GRAPHICSUTILS_TEXTURE_HPP
#define GRAPHICSUTILS_TEXTURE_HPP


#include <string>
#include <glad/glad.h>


namespace gut {

    class Texture {
    public:
        Texture(GLenum target = GL_TEXTURE_2D, GLenum internalFormat = GL_RGBA);
        ~Texture(void);

        Texture(const Texture& other) = delete;
        Texture(Texture&& other) noexcept;
        Texture& operator=(const Texture& other) = delete;
        Texture& operator=(Texture&& other) noexcept;

        // Create empty 2D texture
        void create(int width, int height);
        void create(int width, int height, GLenum target, GLenum internalFormat);
        // Create empty 3D texture
        void create(int width, int height, int depth);
        void create(int width, int height, int depth, GLenum target, GLenum internalFormat);

        // Load texture from an image file
        void loadFromFile(const std::string& fileName);
        void loadFromFile(const std::string& fileName, GLenum target, GLenum internalFormat);

        // Set filtering
        void setFiltering(GLenum minFilter, GLenum magFilter);

        // Set wrapping
        void setWrapping(GLenum sWrap, GLenum tWrap);

        // Bind texture to texture unit
        void bind(GLenum textureUnit = GL_TEXTURE0) const;

        // Bind texture to image unit
        void bindImage(GLuint unit, GLint level = 0, GLenum access = GL_WRITE_ONLY) const;

        // Get dimensions
        int width() const;
        int height() const;
        int depth() const;

        // Get texture ID
        GLuint id() const noexcept;

    private:
        GLuint  _textureId;
        GLenum  _target;
        GLenum  _internalFormat;
        int     _width;
        int     _height;
        int     _depth;

        // Release OpenGL handles and reset Texture state
        void reset(void);
    };

} // namespace gut


#endif //GRAPHICSUTILS_TEXTURE_HPP
