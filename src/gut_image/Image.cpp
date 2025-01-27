//
// Project: GraphicsUtils
// File: Image.cpp
//
// Copyright (c) 2020 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "Image.hpp"
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>


#ifdef __GNUG__
#define INLINE inline __attribute__((always_inline))
#else
#define INLINE inline
#endif


#define CREATE_ARRAY_COPY(TYPE, DST, SRC, SIZE) \
    DST = new TYPE[SIZE]; \
    memcpy(static_cast<TYPE*>(DST), static_cast<TYPE*>(SRC), (SIZE)*sizeof(TYPE));

#define CREATE_ARRAY_COPY_CONVERT(DST_TYPE, DST, SRC_TYPE, SRC, SIZE)                       \
    DST = new DST_TYPE[SIZE];                                                               \
    for (int i=0; i<SIZE; ++i) {                                                            \
        convertPixelValue<SRC_TYPE, DST_TYPE>(                                              \
        (static_cast<SRC_TYPE*>(SRC))[i], (static_cast<DST_TYPE*>(DST))[i]);                \
    }


using namespace gut;


namespace {

    template <typename T_Src, typename T_Dest>
    INLINE void convertPixelValue(T_Src& src, T_Dest& dest) {
        throw std::runtime_error("Could not convert pixel value, unknown type");
    }

    template <>
    INLINE void convertPixelValue(uint8_t& src, uint8_t& dest) {
        dest = src;
    }

    template <>
    INLINE void convertPixelValue(uint16_t& src, uint16_t& dest) {
        dest = src;
    }

    template <>
    INLINE void convertPixelValue(float& src, float& dest) {
        dest = src;
    }

    template <>
    INLINE void convertPixelValue(uint8_t& src, uint16_t& dest) {
        dest = ((uint16_t)src) << 8;
    }

    template <>
    INLINE void convertPixelValue(uint8_t& src, float& dest) {
        dest = (float)src*0.0039215686f;
    }

    template <>
    INLINE void convertPixelValue(uint16_t& src, uint8_t& dest) {
        dest = src >> 8;
    }

    template <>
    INLINE void convertPixelValue(uint16_t& src, float& dest) {
        dest = (float)src*0.000015259021893f;
    }

};


// Pixel and PixelRef member functions
Image::PixelRef::PixelRef(void* data, uint64_t stride, uint64_t rp, uint64_t gp, uint64_t bp, uint64_t ap) :
    data(data), stride(stride), rp(rp), gp(gp), bp(bp), ap(ap)
{
}

Image::PixelRef& Image::PixelRef::operator++()
{
    rp += stride;
    gp += stride;
    bp += stride;
    ap += stride;
    return *this;
}

Image::PixelRef Image::PixelRef::operator++(int)
{
    PixelRef r = *this;
    rp += stride;
    gp += stride;
    bp += stride;
    ap += stride;
    return r;
}


// Image member functions
Image::Image(Image::DataFormat dataFormat, Image::DataType dataType) :
    _dataFormat (dataFormat),
    _dataType   (dataType),
    _width      (0),
    _height     (0),
    _data       (nullptr),
    _deleter    (nullptr)
{
    switch (_dataFormat) {
        case DataFormat::GRAY: {
            int interleave[4] = {0, 0, 0, 0};
            memcpy(_interleave, interleave, 4*sizeof(int));
        }   break;
        case DataFormat::RGB: {
            int interleave[4] = {0, 1, 2, 0};
            memcpy(_interleave, interleave, 4*sizeof(int));
        }   break;
        case DataFormat::RGBA: {
            int interleave[4] = {0, 1, 2, 3};
            memcpy(_interleave, interleave, 4*sizeof(int));
        }   break;
    }
}

Image::Image(const Image& other) :
    _dataFormat (other._dataFormat),
    _dataType   (other._dataType),
    _width      (other._width),
    _height     (other._height),
    _data       (nullptr),
    _deleter    (other._deleter)
{
    memcpy(_interleave, other._interleave, 4*sizeof(int));

    // make a copy of the data vector
    uint64_t s = other._width*other._height*nChannels(other._dataFormat);
    if (other._data != nullptr) {
        switch (_dataType) {
            case DataType::U8:
                CREATE_ARRAY_COPY(uint8_t, _data, other._data, s)
                if (_deleter == nullptr) _deleter = dataDeleter<uint8_t>;
                break;
            case DataType::U16:
                CREATE_ARRAY_COPY(uint16_t, _data, other._data, s)
                if (_deleter == nullptr) _deleter = dataDeleter<uint16_t>;
                break;
            case DataType::F32:
                CREATE_ARRAY_COPY(float, _data, other._data, s)
                if (_deleter == nullptr) _deleter = dataDeleter<float>;
                break;
            default:
                return;
        }
    }
}

Image::Image(Image&& other) noexcept :
    _dataFormat (other._dataFormat),
    _dataType   (other._dataType),
    _width      (other._width),
    _height     (other._height),
    _data       (other._data),
    _deleter    (other._deleter)
{
    memcpy(_interleave, other._interleave, 4*sizeof(int));

    other._width = 0;
    other._height = 0;
    other._data = nullptr;
    other._deleter = nullptr;
}

Image& Image::operator=(const Image& other)
{
    if (_deleter)
        _deleter(_data);

    _dataFormat = other._dataFormat;
    _dataType   = other._dataType;
    _width      = other._width;
    _height     = other._height;
    _data       = nullptr;
    _deleter    = other._deleter;
    memcpy(_interleave, other._interleave, 4*sizeof(int));

    // make a copy of the data vector
    uint64_t s = other._width*other._height*nChannels(other._dataFormat);
    if (other._data != nullptr) {
        switch (_dataType) {
            case DataType::U8:
                CREATE_ARRAY_COPY(uint8_t, _data, other._data, s)
                if (_deleter == nullptr) _deleter = dataDeleter<uint8_t>;
                break;
            case DataType::U16:
                CREATE_ARRAY_COPY(uint16_t, _data, other._data, s)
                if (_deleter == nullptr) _deleter = dataDeleter<uint16_t>;
                break;
            case DataType::F32:
                CREATE_ARRAY_COPY(float, _data, other._data, s)
                if (_deleter == nullptr) _deleter = dataDeleter<float>;
                break;
            default:
                return *this;
        }
    }

    return *this;
}

Image& Image::operator=(Image&& other) noexcept
{
    if (_deleter)
        _deleter(_data);

    _dataFormat = other._dataFormat;
    _dataType   = other._dataType;
    _width      = other._width;
    _height     = other._height;
    _data       = other._data;
    _deleter    = other._deleter;
    memcpy(_interleave, other._interleave, 4*sizeof(int));

    other._width = 0;
    other._height = 0;
    other._data = nullptr;
    other._deleter = nullptr;

    return *this;
}

Image::~Image()
{
    if (_deleter)
        _deleter(_data);
}

void Image::create(int width, int height)
{
    // do nothing in case width or height has not changed
    if (width == _width && height == _height)
        return;

    // check for invalid dimensions
    if (width < 0 || height < 0) {
        fprintf(stderr, "Failed to create image: Invalid dimensions\n"); // TODO logging
        return;
    }

    // free previous data
    if (_deleter)
        _deleter(_data);

    // data size
    uint64_t s = width*height*nChannels(_dataFormat);

    switch (_dataType) {
        case DataType::U8:
            _data = new uint8_t[s];
            _deleter = dataDeleter<uint8_t>;
            break;
        case DataType::U16:
            _data = new uint16_t[s];
            _deleter = dataDeleter<uint16_t>;
            break;
        case DataType::F32:
            _data = new float[s];
            _deleter = dataDeleter<float>;
            break;
        default:
            return;
    }

    _width = width;
    _height = height;
}

void Image::loadFromFile(const std::string& fileName)
{
    int imgChannels;
    void* imgData = nullptr;

    if (stbi_is_16_bit(fileName.c_str())) { // 16 bit image
        _dataType = DataType::U16;
        imgData = stbi_load_16(fileName.c_str(), &_width, &_height, &imgChannels, 0);
    }
    else if (stbi_is_hdr(fileName.c_str())) { // HDR image
        _dataType = DataType::F32;
        imgData = stbi_loadf(fileName.c_str(), &_width, &_height, &imgChannels, 0);
    }
    else { // 8 bit image
        _dataType = DataType::U8;
        imgData = stbi_load(fileName.c_str(), &_width, &_height, &imgChannels, 0);
    }

    // Check for errors
    if (imgData == nullptr) {
        fprintf(stderr, "Failed to load image %s\n", fileName.c_str()); // TODO logging
        return;
    }

    if (imgChannels != 1 && imgChannels != 3 && imgChannels != 4) {
        fprintf(stderr, "Failed to load image %s: unsupported format\n", fileName.c_str()); // TODO logging
        return;
    }

    // Set data type and format
    switch (imgChannels) {
        case 1:
            _dataFormat = DataFormat::GRAY;
            break;
        case 3:
            _dataFormat = DataFormat::RGB;
            break;
        case 4:
            _dataFormat = DataFormat::RGBA;
            break;
        default:
            break;
    }

    // Copy image data and release resources
    switch (_dataType) {
        case DataType::U8:
            CREATE_ARRAY_COPY(uint8_t, _data, imgData, _width * _height * imgChannels);
            _deleter = dataDeleter<uint8_t>;
            stbi_image_free((stbi_uc*)imgData);
            break;
        case DataType::U16:
            CREATE_ARRAY_COPY(uint16_t, _data, imgData, _width * _height * imgChannels);
            _deleter = dataDeleter<uint16_t>;
            stbi_image_free((stbi_us*)imgData);
            break;
        case DataType::F32:
            CREATE_ARRAY_COPY(float, _data, imgData, _width * _height * imgChannels);
            _deleter = dataDeleter<float>;
            stbi_image_free((float*)imgData);
            break;
    }

}

void Image::writeToFile(const std::string& fileName)
{
    std::string ext = fileName.substr(fileName.size()-3, 3);

    if (ext == "png" || ext == "PNG") {
        switch(_dataType) {
            case DataType::U8:
                stbi_write_png(fileName.c_str(), _width, _height, nChannels(_dataFormat),
                    static_cast<uint8_t*>(_data), 0);
                break;
            case DataType::U16:
                // This is here for the future 16-bit support in STB
                fprintf(stderr, "ERROR: Unable to save PNG: 16-bit write not yet supported.\n"); // TODO logging
                break;
            default:
                fprintf(stderr, "ERROR: Unable to save PNG: invalid data type.\n"); // TODO logging
                return;
        }
    }
    else if (ext == "bmp" || ext == "BMP") {
        switch(_dataType) {
            case DataType::U8:
                stbi_write_bmp(fileName.c_str(), _width, _height, nChannels(_dataFormat),
                    static_cast<uint8_t*>(_data));
                break;
            default:
                fprintf(stderr, "ERROR: Unable to save BMP: invalid data type.\n"); // TODO logging
                return;
        }
    }
    else if (ext == "jpg" || ext == "JPG") {
        switch(_dataType) {
            case DataType::U8:
                stbi_write_jpg(fileName.c_str(), _width, _height, nChannels(_dataFormat),
                    static_cast<uint8_t*>(_data), 100);
                break;
            default:
                fprintf(stderr, "ERROR: Unable to save JPG: invalid data type.\n"); // TODO logging
                return;
        }
    }
    else if (ext == "tga" || ext == "TGA") {
        switch(_dataType) {
            case DataType::U8:
                stbi_write_tga(fileName.c_str(), _width, _height, nChannels(_dataFormat),
                    static_cast<uint8_t*>(_data));
                break;
            default:
                fprintf(stderr, "ERROR: Unable to save TGA: invalid data type.\n"); // TODO logging
                return;
        }
    }
    else if (ext == "hdr" || ext == "HDR") {
        switch(_dataType) {
            case DataType::F32:
                stbi_write_hdr(fileName.c_str(), _width, _height, nChannels(_dataFormat),
                    static_cast<float*>(_data));
                break;
            default:
                fprintf(stderr, "ERROR: Unable to save HDR: invalid data type.\n"); // TODO logging
                return;
        }
    }
}

void Image::convertDataType(Image::DataType dataType)
{
    if (_data == nullptr)
        return;

    void* newData;
    int s = _width*_height* nChannels(_dataFormat);
    switch (dataType) {
        case DataType::U8:
            switch (_dataType) {
                case DataType::U8:
                    return;
                case DataType::U16:
                    CREATE_ARRAY_COPY_CONVERT(uint8_t, newData, uint16_t, _data, s)
                    break;
                case DataType::F32:
                    CREATE_ARRAY_COPY_CONVERT(uint8_t, newData, float, _data, s)
                    break;
                default:
                    return;
            }
            if (_deleter)
                _deleter(_data);
            _deleter = dataDeleter<uint8_t>;
            break;
        case DataType::U16:
            switch (_dataType) {
                case DataType::U8:
                    CREATE_ARRAY_COPY_CONVERT(uint16_t, newData, uint8_t, _data, s)
                    break;
                case DataType::U16:
                    return;
                case DataType::F32:
                    CREATE_ARRAY_COPY_CONVERT(uint16_t, newData, float, _data, s)
                    break;
                default:
                    return;
            }
            if (_deleter)
                _deleter(_data);
            _deleter = dataDeleter<uint16_t>;
            break;
        case DataType::F32:
            switch (_dataType) {
                case DataType::U8:
                    CREATE_ARRAY_COPY_CONVERT(float, newData, uint8_t, _data, s)
                    break;
                case DataType::U16:
                    CREATE_ARRAY_COPY_CONVERT(float, newData, uint16_t, _data, s)
                    break;
                case DataType::F32:
                    return;
                default:
                    return;
            }
            if (_deleter)
                _deleter(_data);
            _deleter = dataDeleter<float>;
            break;
        default:
            return;
    }
    _data = newData;
    _dataType = dataType;
}

Image::DataFormat Image::dataFormat() const noexcept
{
    return _dataFormat;
}

Image::DataType Image::dataType() const noexcept
{
    return _dataType;
}

int Image::width() const noexcept
{
    return _width;
}

int Image::height() const noexcept
{
    return _height;
}

Image::PixelRef Image::operator()(int x, int y)
{
    uint64_t p = (y*_width + x)*nChannels(_dataFormat);
    PixelRef pRef(_data, nChannels(_dataFormat),
        p+_interleave[0], p+_interleave[1], p+_interleave[2], p+_interleave[3]);
    return pRef;
}

const Image::PixelRef Image::operator()(int x, int y) const
{
    uint64_t p = (y*_width + x)*nChannels(_dataFormat);
    PixelRef pRef(_data, nChannels(_dataFormat),
        p+_interleave[0], p+_interleave[1], p+_interleave[2], p+_interleave[3]);
    return pRef;
}
