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

#include <cstdint>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


using namespace gut;


constexpr int Image::nChannels(Image::DataFormat dataFormat)
{
    switch (dataFormat) {
        case DataFormat::RGB:   return 3;
        case DataFormat::RGBA:  return 4;
    }

    return -1;
}

Image::Image(Image::DataFormat dataFormat, Image::DataType dataType) :
    _dataFormat (dataFormat),
    _dataType   (dataType),
    _width      (0),
    _height     (0),
    _data       (nullptr),
    _deleter    (nullptr)
{
}

Image::Image(const Image& other) :
    _dataFormat (other._dataFormat),
    _dataType   (other._dataType),
    _width      (other._width),
    _height     (other._height),
    _data       (nullptr),
    _deleter    (other._deleter)
{
    // make a copy of the data vector
    if (other._data != nullptr) {
        switch (_dataType) {
            case DataType::U8:
                _data = new std::vector<uint8_t>(*static_cast<std::vector<uint8_t>*>(other._data));
                break;
            case DataType::U16:
                _data = new std::vector<uint16_t>(*static_cast<std::vector<uint16_t>*>(other._data));
                break;
            case DataType::F32:
                _data = new std::vector<float>(*static_cast<std::vector<float>*>(other._data));
                break;
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
    other._width = 0;
    other._height = 0;
    other._data = nullptr;
    other._deleter = nullptr;
}

Image& Image::operator=(const Image& other)
{
    _dataFormat = other._dataFormat;
    _dataType   = other._dataType;
    _width      = other._width;
    _height     = other._height;
    _data       = nullptr;
    _deleter    = other._deleter;

    // make a copy of the data vector
    if (other._data != nullptr) {
        switch (_dataType) {
            case DataType::U8:
                _data = new std::vector<uint8_t>(*static_cast<std::vector<uint8_t>*>(other._data));
                break;
            case DataType::U16:
                _data = new std::vector<uint16_t>(*static_cast<std::vector<uint16_t>*>(other._data));
                break;
            case DataType::F32:
                _data = new std::vector<float>(*static_cast<std::vector<float>*>(other._data));
                break;
        }
    }

    return *this;
}

Image& Image::operator=(Image&& other) noexcept
{
    _dataFormat = other._dataFormat;
    _dataType   = other._dataType;
    _width      = other._width;
    _height     = other._height;
    _data       = other._data;
    _deleter    = other._deleter;

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
    if (width < 0 || height < 0) {
        fprintf(stderr, "Failed to create image: Invalid dimensions\n"); // TODO logging
        return;
    }

    switch (_dataType) {
        case DataType::U8:
            _data = new std::vector<uint8_t>(width*height*nChannels(_dataFormat), 0);
            _deleter = dataDeleter<uint8_t>;
            break;
        case DataType::U16:
            _data = new std::vector<uint16_t>(width*height*nChannels(_dataFormat), 0);
            _deleter = dataDeleter<uint16_t>;
            break;
        case DataType::F32:
            _data = new std::vector<float>(width*height*nChannels(_dataFormat), 0.0f);
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
    uint8_t *imgData = stbi_load(fileName.c_str(), &_width, &_height, &imgChannels, 0);

    // Check for errors
    if (imgData == nullptr) {
        fprintf(stderr, "Failed to load image %s\n", fileName.c_str()); // TODO logging
        return;
    }

    if (imgChannels != 3 && imgChannels != 4) {
        fprintf(stderr, "Failed to load image %s: unsupported format\n", fileName.c_str()); // TODO logging
        return;
    }

    // Set data type and format
    _dataType = DataType::U8;
    switch (imgChannels) {
        case 3:
            _dataFormat = DataFormat::RGB;
            break;
        case 4:
            _dataFormat = DataFormat::RGBA;
            break;
        default:
            break;
    }

    // Setup the data vector and copy the data
    // TODO investigate possibility of directly loading image to vector to avoid copy
    int64_t vSize = _width*_height*nChannels(_dataFormat);
    _data = new std::vector<uint8_t>(vSize);
    _deleter = dataDeleter<uint8_t>;

    auto& v = *static_cast<std::vector<uint8_t>*>(_data);
    for (int64_t i=0; i<vSize; ++i) {
        v[i] = imgData[i];
    }

    // Free up resources
    stbi_image_free(imgData);
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
