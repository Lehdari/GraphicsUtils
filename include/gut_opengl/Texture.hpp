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

#include <gut_image/Image.hpp>
#include <gut_opengl/GLTypeUtils.hpp>


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

        /** @brief  Create an empty texture with the current channel format and data type
         *  @param  width   Desired width of the texture
         *  @param  height  Desired height of the texture
         *  @param  depth   Desired depth of the texture (0 in case of a 2D texture)
         */
        void create(int width, int height, int depth = 0);

        /** @brief  Create an empty 2D texture
         *  @param  width           Desired width of the texture
         *  @param  height          Desired height of the texture
         *  @param  target          OpenGL texture target
         *  @param  channelFormat   Internal color channel format
         *  @param  dataType        Internal data type
         */
        void create(int width, int height,
            GLenum target, GLenum channelFormat, GLenum dataType);

        /** @brief  Create an empty 3D texture
         *  @param  width           Desired width of the texture
         *  @param  height          Desired height of the texture
         *  @param  depth           Desired depth of the texture
         *  @param  target          OpenGL texture target
         *  @param  channelFormat   Internal color channel format
         *  @param  dataType        Internal data type
         */
        void create(int width, int height, int depth,
            GLenum target, GLenum channelFormat, GLenum dataType);

        // Load texture from an image file
        void loadFromFile(const std::string& fileName);
        void loadFromFile(const std::string& fileName, GLenum dataType);
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

        /** @brief  Update Texture from a raw data buffer
         *  @tparam T_Data  Buffer data type, must be supported by typeToGLEnum()
         *  @param  buffer  Buffer to copy the texture data from
         *  @param  format  Buffer color channel format
         *  @param  width   Width of the texture stored in the buffer
         *  @param  height  Height of the texture stored in the buffer
         *  @note   Target and internal format defined in constructor or create() are used
         */
        template <typename T_Data>
        void updateFromBuffer(const T_Data* buffer, GLenum format, int width=-1, int height=-1);

        /** @brief  Read the texture from the GPU and copy it to an image
         *  @param  image   Target Image object
         *  @param  level   Level of texture to copy
         */
        void copyToImage(Image& image, GLint level = 0) const;

        /** @brief  Initiate pixel transfer from GPU via PBO for mapToImage(). Non-blocking operation.
         */
        void initiateMapping();

        /** @brief  Map the texture to an image via PBO
         *  @param  access  Access policy, supports GL_READ_ONLY and GL_READ_WRITE
         *  @return Image object with the mapped pixels
         *  @note   Consider calling initiateMapping() earlier to enable asynchronous pixel transfer
         *  @note   unmap() must be called once operations on the image are done and return
         *          to normal GL pixel operations is required.
         */
        gut::Image& mapToImage(GLenum access = GL_READ_WRITE);

        /** @brief  Unmap the the texture from the image and return to normal GL
         *          pixel operations
         */
        void unmap();

        /** @brief  Enable double buffering on the texture. Useful when the texture is being
         *          modified by for example, compute shaders
         */
        void enableDoubleBuffering();

        /** @brief  Generate mipmaps
         */
        void generateMipMaps();

        // Set filtering
        void setFiltering(GLenum minFilter, GLenum magFilter);

        // Set wrapping
        void setWrapping(GLenum sWrap, GLenum tWrap);

        // Bind texture to texture unit
        void bind(GLenum textureUnit = GL_TEXTURE0) const;

        /** @brief  Swap buffers (in case the texture is double buffered)
         */
        void swap();

        // Bind texture to image unit
        void bindImage(GLuint unit, GLint level = 0, GLenum access = GL_WRITE_ONLY,
            bool active = true) const;

        // Get dimensions
        int width() const;
        int height() const;
        int depth() const;

        /** @brief  Get texture ID
         *  @return OpenGL ID of the texture
         *  @note   When the texture is double buffered, id of the active texture is returned
         */
        GLuint id() const noexcept;

        friend class FrameBuffer;

    private:
        int         _activeId; // index in _textureIds array, required when double buffering
        GLuint      _textureIds[2];
        GLenum      _target;
        GLenum      _channelFormat;
        GLenum      _dataType;
        int         _width;
        int         _height;
        int         _depth;
        bool        _doubleBuffered;
        GLuint      _pboId;
        gut::Image  _mappedImage;
        GLenum      _mappedImageAccess;

        // Release OpenGL handles and reset Texture state
        void reset(int id = -1);
    };


    #include "Texture.inl"

} // namespace gut


#endif //GRAPHICSUTILS_TEXTURE_HPP
