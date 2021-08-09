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
#include <gut_image/Image.hpp>
#include <gut_opengl/GLTypeUtils.hpp>
#include <stdexcept>


using namespace gut;


Texture::Texture(GLenum target, GLenum channelFormat, GLenum dataType) :
    _activeId           (0), // always point to first texture id in case double buffering is disabled
    _textureIds         {0, 0},
    _target             (target),
    _channelFormat      (channelFormat),
    _dataType           (dataType),
    _width              (0),
    _height             (0),
    _depth              (0),
    _doubleBuffered     (false),
    _pboId              (0),
    _mappedImageAccess  (GL_READ_WRITE)
{
}

Texture::~Texture(void)
{
    reset();
}

Texture::Texture(Texture&& other) noexcept :
    _activeId           (other._activeId),
    _textureIds         {other._textureIds[0], other._textureIds[1]},
    _target             (other._target),
    _channelFormat      (other._channelFormat),
    _dataType           (other._dataType),
    _width              (other._width),
    _height             (other._height),
    _depth              (other._depth),
    _doubleBuffered     (other._doubleBuffered),
    _pboId              (other._pboId),
    _mappedImageAccess  (other._mappedImageAccess)
{
    other._activeId = 0;
    other._textureIds[0] = 0;
    other._textureIds[1] = 0;
    other._width = 0;
    other._height = 0;
    other._depth = 0;
    other._doubleBuffered = false;
    other._pboId = 0;
    other._mappedImageAccess = GL_READ_WRITE;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    _activeId = other._activeId;
    _textureIds[0] = other._textureIds[0];
    _textureIds[1] = other._textureIds[1];
    _target = other._target;
    _channelFormat = other._channelFormat;
    _dataType = other._dataType;
    _width = other._width;
    _height = other._height;
    _depth = other._depth;
    _doubleBuffered = other._doubleBuffered;
    _pboId = other._pboId;
    _mappedImageAccess = other._mappedImageAccess;

    other._activeId = 0;
    other._textureIds[0] = 0;
    other._textureIds[1] = 0;
    other._width = 0;
    other._height = 0;
    other._depth = 0;
    other._doubleBuffered = false;
    other._pboId = 0;
    other._mappedImageAccess = GL_READ_WRITE;
    
    return *this;
}

void Texture::create(int width, int height, int depth)
{
    if (depth == 0)
        create(width, height, _target, _channelFormat, _dataType);
    else
        create(width, height, depth, _target, _channelFormat, _dataType);
}

void Texture::create(int width, int height, GLenum target, GLenum channelFormat, GLenum dataType)
{
    _width = width;
    _height = height;
    _target = target;
    _channelFormat = channelFormat;
    _dataType = dataType;

    // Release the used resources (both in case double buffering is in use)
    reset(_doubleBuffered ? -1 : _activeId);

    // Generate new texture
    glGenTextures(1, &_textureIds[_activeId]);
    glBindTexture(_target, _textureIds[_activeId]);

    // Set filtering and wrapping
    glTexParameteri(_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = GL_RGBA;
    if (channelFormat == GL_DEPTH_COMPONENT ||
        channelFormat == GL_DEPTH_STENCIL)
        format = channelFormat;

    // Transfer data to OpenGL and generate mipmaps
    glTexImage2D(_target, 0, _channelFormat, _width, _height, 0, format, _dataType, nullptr);

    glGenerateMipmap(_target);

    glBindTexture(_target, 0);

    // Initialize the other texture also in case double buffering is used
    if (_doubleBuffered) {
        _doubleBuffered = false;
        _activeId ^= 1;
        create(_width, _height, _depth);
        _doubleBuffered = true;
        _activeId ^= 1;
    }
}

void Texture::create(int width, int height, int depth,
    GLenum target, GLenum channelFormat, GLenum dataType)
{
    _width = width;
    _height = height;
    _depth = depth;
    _target = target;
    _channelFormat = channelFormat;
    _dataType = dataType;

    // Release the used resources (both in case double buffering is in use)
    reset(_doubleBuffered ? -1 : _activeId);

    // Generate new texture
    glGenTextures(1, &_textureIds[_activeId]);
    glBindTexture(_target, _textureIds[_activeId]);

    // Set filtering and wrapping
    glTexParameteri(_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Transfer data to OpenGL and generate mipmaps
    glTexImage3D(_target, 0, _channelFormat, _width, _height, _depth, 0, GL_RGBA, _dataType, nullptr);

    glGenerateMipmap(_target);

    glBindTexture(_target, 0);

    // Initialize the other texture also in case double buffering is used
    if (_doubleBuffered) {
        _doubleBuffered = false;
        _activeId ^= 1;
        create(_width, _height, _depth);
        _doubleBuffered = true;
        _activeId ^= 1;
    }
}

void Texture::loadFromFile(const std::string& fileName)
{
    loadFromFile(fileName, _target, _channelFormat);
}

void Texture::loadFromFile(const std::string& fileName, GLenum dataType)
{
    Image img;
    img.loadFromFile(fileName);
    _dataType = dataType;
    img.convertDataType(glEnumToImageDataType(_dataType));
    loadFromImage(img, _target, _channelFormat);
}

void Texture::loadFromFile(const std::string& fileName, GLenum target, GLenum channelFormat)
{
    Image img;
    img.loadFromFile(fileName);
    loadFromImage(img, target, channelFormat);
}

void Texture::loadFromImage(const Image& image)
{
    loadFromImage(image, _target, _channelFormat);
}

void Texture::loadFromImage(const Image& image, GLenum target, GLenum channelFormat)
{
    _width = image.width();
    _height = image.height();
    _depth = 0;

    _target = target;
    _channelFormat = channelFormat;

    switch (image.dataType()) {
        case Image::DataType::U8:
            _dataType = GL_UNSIGNED_BYTE;
            break;
        case Image::DataType::U16:
            _dataType = GL_UNSIGNED_SHORT;
            break;
        case Image::DataType::F32:
            _dataType = GL_FLOAT;
            break;
        default:
            throw std::runtime_error("ERROR: Texture::loadFromImage(): Invalid image data type");
    }

    // Release the used resources (both in case double buffering is in use)
    reset(_doubleBuffered ? -1 : _activeId);

    // Generate new texture
    glGenTextures(1, &_textureIds[_activeId]);
    glBindTexture(_target, _textureIds[_activeId]);

    // Set filtering and wrapping
    glTexParameteri(_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Transfer data to OpenGL and generate mipmaps
    switch (image.dataFormat()) {
        case Image::DataFormat::GRAY:
            switch (image.dataType()) {
                case Image::DataType::U8:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RED, _dataType, image.data<uint8_t>());
                    break;
                case Image::DataType::U16:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RED, _dataType, image.data<uint16_t>());
                    break;
                case Image::DataType::F32:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RED, _dataType, image.data<float>());
                    break;
                default:
                    throw std::runtime_error("ERROR: Texture::loadFromImage(): Invalid image data type");
            }
            break;
        case Image::DataFormat::RGB:
            switch (image.dataType()) {
                case Image::DataType::U8:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGB, _dataType, image.data<uint8_t>());
                    break;
                case Image::DataType::U16:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGB, _dataType, image.data<uint16_t>());
                    break;
                case Image::DataType::F32:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGB, _dataType, image.data<float>());
                    break;
                default:
                    throw std::runtime_error("ERROR: Texture::loadFromImage(): Invalid image data type");
            }
            break;
        case Image::DataFormat::RGBA:
            switch (image.dataType()) {
                case Image::DataType::U8:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGBA, _dataType, image.data<uint8_t>());
                    break;
                case Image::DataType::U16:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGBA, _dataType, image.data<uint16_t>());
                    break;
                case Image::DataType::F32:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGBA, _dataType, image.data<float>());
                    break;
                default:
                    throw std::runtime_error("ERROR: Texture::loadFromImage(): Invalid image data type");
            }
            break;
        default:
            break;
    }
    glGenerateMipmap(_target);

    glBindTexture(_target, 0);

    // Initialize the other texture also in case double buffering is used
    if (_doubleBuffered) {
        _doubleBuffered = false;
        _activeId ^= 1;
        create(_width, _height, _depth);
        _doubleBuffered = true;
        _activeId ^= 1;
    }
}

void Texture::updateFromImage(const Image& image)
{
    _width = image.width();
    _height = image.height();

    switch (image.dataType()) {
        case Image::DataType::U8:
            _dataType = GL_UNSIGNED_BYTE;
            break;
        case Image::DataType::U16:
            _dataType = GL_UNSIGNED_SHORT;
            break;
        case Image::DataType::F32:
            _dataType = GL_FLOAT;
            break;
        default:
            throw std::runtime_error("ERROR: Texture::updateFromImage(): Invalid image data type");
    }

    glBindTexture(_target, _textureIds[_activeId]);

    // Transfer data to OpenGL and generate mipmaps
    switch (image.dataFormat()) {
        case Image::DataFormat::GRAY:
            switch (image.dataType()) {
                case Image::DataType::U8:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RED, _dataType, image.data<uint8_t>());
                    break;
                case Image::DataType::U16:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RED, _dataType, image.data<uint16_t>());
                    break;
                case Image::DataType::F32:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RED, _dataType, image.data<float>());
                    break;
                default:
                    throw std::runtime_error("ERROR: Texture::updateFromImage(): Invalid image data type");
            }
            break;
        case Image::DataFormat::RGB:
            switch (image.dataType()) {
                case Image::DataType::U8:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGB, _dataType, image.data<uint8_t>());
                    break;
                case Image::DataType::U16:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGB, _dataType, image.data<uint16_t>());
                    break;
                case Image::DataType::F32:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGB, _dataType, image.data<float>());
                    break;
                default:
                    throw std::runtime_error("ERROR: Texture::updateFromImage(): Invalid image data type");
            }
            break;
        case Image::DataFormat::RGBA:
            switch (image.dataType()) {
                case Image::DataType::U8:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGBA, _dataType, image.data<uint8_t>());
                    break;
                case Image::DataType::U16:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGBA, _dataType, image.data<uint16_t>());
                    break;
                case Image::DataType::F32:
                    glTexImage2D(_target, 0, _channelFormat, _width, _height,
                        0, GL_RGBA, _dataType, image.data<float>());
                    break;
                default:
                    throw std::runtime_error("ERROR: Texture::updateFromImage(): Invalid image data type");
            }
            break;
        default:
            break;
    }
    glGenerateMipmap(_target);

    glBindTexture(_target, 0);
}

void Texture::copyToImage(Image& image, GLint level) const
{
    glBindTexture(_target, _textureIds[_activeId]);

    // copy dimensions, depending on the mipmap level
    int wCopy = _width >> level;
    int hCopy = _height >> level;

    image.create(wCopy, hCopy);

    glGetnTexImage(_target, level,
        imageDataFormatToGLEnum(image.dataFormat()),
        imageDataTypeToGLEnum(image.dataType()),
        wCopy*hCopy*Image::nChannels(image.dataFormat())*Image::dataTypeSize(image.dataType()),
        image.data<void>());
}

void Texture::initiateMapping()
{
    glBindTexture(_target, _textureIds[_activeId]);

    // supported only for 2D textures
    if (_target != GL_TEXTURE_2D)
        throw std::runtime_error("mapToImage is only supported for GL_TEXTURE_2D target");

    // initialize image
    _mappedImage = gut::Image(
        glEnumToImageDataFormat(_channelFormat),
        glEnumToImageDataType(_dataType));

    // initialize PBOs in case they're uninitialized
    if (_pboId == 0) {
        size_t pboSize = _width*_height*glDataTypeSize(_dataType)*glNumberOfChannels(_channelFormat);
        glGenBuffers(1, &_pboId);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, _pboId);
        glBufferData(GL_PIXEL_PACK_BUFFER, pboSize, NULL, GL_STREAM_READ);
    }

    // Transfer the pixel data using a PBO
    glBindBuffer(GL_PIXEL_PACK_BUFFER, _pboId);
    glBindTexture(GL_TEXTURE_2D, _textureIds[_activeId]);
    glGetTexImage(GL_TEXTURE_2D, 0, imageDataFormatToGLEnum(_mappedImage._dataFormat), _dataType, nullptr);
}

gut::Image& Texture::mapToImage(GLenum access)
{
    // check that GL_PIXEL_PACK_BUFFER is bound
    GLint boundBufferId = 0;
    glGetIntegerv(GL_PIXEL_PACK_BUFFER_BINDING, &boundBufferId);
    if (boundBufferId == 0) {
        // not bound, do just-in-time pixel transfer
        initiateMapping();
    }

    // Map the texture to an image
    _mappedImageAccess = access;
    void* ptr = glMapBuffer(GL_PIXEL_PACK_BUFFER, _mappedImageAccess);
    _mappedImage._data = ptr;
    _mappedImage._width = _width;
    _mappedImage._height = _height;

    return _mappedImage;
}

void Texture::unmap()
{
    // reset the mapped image
    _mappedImage._data = nullptr;
    _mappedImage._width = 0;
    _mappedImage._height = 0;

    // unmap
    glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    // upload pixel data in case of read/write mapping
    if (_mappedImageAccess == GL_READ_WRITE) {
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, _pboId);
        glBindTexture(GL_TEXTURE_2D, _textureIds[_activeId]);
        glTexImage2D(GL_TEXTURE_2D, 0, _channelFormat, _width, _height, 0,
            imageDataFormatToGLEnum(_mappedImage._dataFormat), _dataType, 0);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    }
}

void Texture::enableDoubleBuffering()
{
    if (_doubleBuffered)
        return;

    _activeId ^= 1;
    create(_width, _height, _depth);
    _activeId ^= 1;

    _doubleBuffered = true;
}

void Texture::generateMipMaps()
{
    glBindTexture(_target, _textureIds[_activeId]);
    glGenerateMipmap(_target);
}

void Texture::setFiltering(GLenum minFilter, GLenum magFilter)
{
    glBindTexture(_target, _textureIds[_activeId]);

    // Set filtering
    glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, magFilter);

    if (_doubleBuffered) {
        glBindTexture(_target, _textureIds[_activeId^1]);

        // Set filtering
        glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, magFilter);
    }
}

void Texture::setWrapping(GLenum sWrap, GLenum tWrap)
{
    glBindTexture(_target, _textureIds[_activeId]);

    // Set filtering
    glTexParameteri(_target, GL_TEXTURE_WRAP_S, sWrap);
    glTexParameteri(_target, GL_TEXTURE_WRAP_T, tWrap);

    if (_doubleBuffered) {
        glBindTexture(_target, _textureIds[_activeId^1]);

        // Set filtering
        glTexParameteri(_target, GL_TEXTURE_WRAP_S, sWrap);
        glTexParameteri(_target, GL_TEXTURE_WRAP_T, tWrap);
    }
}

void Texture::bind(GLenum textureUnit) const
{
    glActiveTexture(textureUnit);
    glBindTexture(_target, _textureIds[_activeId]);
}

void Texture::swap()
{
    if (!_doubleBuffered)
        throw std::runtime_error("Trying to swap texture with no double buffering");

    _activeId ^= 1;
}

void Texture::bindImage(GLuint unit, GLint level, GLenum access, bool active) const
{
    if (_depth > 0) {
        glBindImageTexture(unit, _textureIds[active ? _activeId : _activeId^1],
            level, GL_TRUE, 0, access, _channelFormat);
    }
    else {
        glBindImageTexture(unit, _textureIds[active ? _activeId : _activeId^1],
            level, GL_FALSE, 0, access, _channelFormat);
    }
}

int Texture::width() const
{
    return _width;
}

int Texture::height() const
{
    return _height;
}

int Texture::depth() const
{
    return _depth;
}

GLuint Texture::id() const noexcept
{
    return _textureIds[_activeId];
}

void Texture::reset(int id)
{
    if (id < 0) {
        if (_textureIds[0] != 0)
            glDeleteTextures(1, &_textureIds[0]);
        if (_textureIds[1] != 0)
            glDeleteTextures(1, &_textureIds[1]);
    }
    else {
        if (_textureIds[id] != 0)
            glDeleteTextures(1, &_textureIds[id]);
    }

    if (_pboId != 0)
        glDeleteBuffers(1, &_pboId);
}
