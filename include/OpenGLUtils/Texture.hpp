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
        Texture(void);
        ~Texture(void);

        Texture(const Texture& other) = delete;
        Texture(Texture&& other) noexcept;
        Texture& operator=(const Texture& other) = delete;
        Texture& operator=(Texture&& other) noexcept;

        // Load texture from an image file
        void loadFromFile(const std::string& fileName, GLenum internalFormat = GL_RGBA);

        // Bind texture to texture unit
        void bind(GLenum textureUnit = GL_TEXTURE0) const;

    private:
        GLuint  _textureId;

        // Release OpenGL handles and reset Texture state
        void reset(void);
    };

} // namespace gut


#endif //GRAPHICSUTILS_TEXTURE_HPP
