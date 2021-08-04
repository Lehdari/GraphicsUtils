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


    class Image;


    class Texture {
    public:
        /** @brief  Construct a Texture object
         *  @param  target          Texture target (type)
         *  @param  channelFormat   Internal color channel format
         *  @param  dataType        Internal data type
         */
        Texture(
            GLenum target = GL_TEXTURE_2D,
            GLenum channelFormat = GL_RGBA,
            GLenum dataType = GL_UNSIGNED_BYTE);
        ~Texture(void);

        Texture(const Texture& other) = delete;
        Texture(Texture&& other) noexcept;
        Texture& operator=(const Texture& other) = delete;
        Texture& operator=(Texture&& other) noexcept;

        // Create empty 2D texture
        void create(int width, int height);
        void create(int width, int height,
            GLenum target, GLenum channelFormat, GLenum dataType);
        // Create empty 3D texture
        void create(int width, int height, int depth);
        void create(int width, int height, int depth,
            GLenum target, GLenum channelFormat, GLenum dataType);

        // Load texture from an image file
        void loadFromFile(const std::string& fileName);
        void loadFromFile(const std::string& fileName, GLenum target, GLenum channelFormat);

        /** @brief  Load Texture from Image object
         *  @param  image   Image object to load image file from
         *  @note   Target and internal format defined in constructor are used
         */
        void loadFromImage(const Image& image);

        /** @brief  Load Texture from Image object and set target and internal format
         *  @param  image           Image object to load image file from
         *  @param  target          Texture target (type)
         *  @param  channelFormat   Internal color channel format
         */
        void loadFromImage(const Image& image, GLenum target, GLenum channelFormat);

        /** @brief  Update Texture from Image object
         *  @param  image   Image object to update image file from
         *  @note   Target and internal format defined in constructor are used
         */
        void updateFromImage(const Image& image);

        /** @brief  Read the texture from the GPU and copy it to an image
         *  @param  image   Target Image object
         *  @param  level   Level of texture to copy
         */
        void copyToImage(Image& image, GLint level = 0) const;

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

        friend class FrameBuffer;

    private:
        GLuint  _textureId;
        GLenum  _target;
        GLenum  _channelFormat;
        GLenum  _dataType;
        int     _width;
        int     _height;
        int     _depth;

        // Release OpenGL handles and reset Texture state
        void reset(void);
    };

} // namespace gut


#endif //GRAPHICSUTILS_TEXTURE_HPP
