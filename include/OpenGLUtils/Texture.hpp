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
#include "GL/gl3w.h"


namespace gut {

    class Texture {
    public:
        Texture(GLenum internalFormat = GL_RGBA);
        ~Texture(void);

        Texture(const Texture& other) = delete;
        Texture(Texture&& other) noexcept;
        Texture& operator=(const Texture& other) = delete;
        Texture& operator=(Texture&& other) noexcept;

        // Create empty texture
        void create(int width, int height);
        void create(int width, int height, GLenum internalFormat);

        // Load texture from an image file
        void loadFromFile(const std::string& fileName);
        void loadFromFile(const std::string& fileName, GLenum internalFormat);

        // Set filtering
        void setFiltering(GLenum minFilter, GLenum magFilter);

        // Set wrapping
        void setWrapping(GLenum sWrap, GLenum tWrap);

        // Bind texture to texture unit
        void bind(GLenum textureUnit = GL_TEXTURE0) const;

        // Bind texture to image unit
        void bindImage(GLuint unit, GLenum access = GL_WRITE_ONLY) const;

        // Get dimensions
        int width() const;
        int height() const;

    private:
        GLuint  _textureId;
        GLenum  _internalFormat;
        int     _width;
        int     _height;

        // Release OpenGL handles and reset Texture state
        void reset(void);
    };

} // namespace gut


#endif //GRAPHICSUTILS_TEXTURE_HPP
