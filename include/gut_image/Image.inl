//
// Project: GraphicsUtils
// File: Image.inl
//
// Copyright (c) 2020 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

// Constexpr functions
constexpr int Image::nChannels(Image::DataFormat dataFormat)
{
    switch (dataFormat) {
        case DataFormat::GRAY:  return 1;
        case DataFormat::RGB:   return 3;
        case DataFormat::RGBA:  return 4;
    }

    return -1;
}

template <typename T_Data>
constexpr Image::DataType Image::dataTypeEnum()
{
    return DataType::INVALID;
}

template <>
constexpr Image::DataType Image::dataTypeEnum<uint8_t>()
{
    return DataType::U8;
}

template <>
constexpr Image::DataType Image::dataTypeEnum<uint16_t>()
{
    return DataType::U16;
}

template <>
constexpr Image::DataType Image::dataTypeEnum<float>()
{
    return DataType::F32;
}


// Pixel and PixelRef template member functions
template<typename T_Data>
Image::Pixel<T_Data>::Pixel(T_Data r, T_Data g, T_Data b, T_Data a) :
    r(r),   g(g),   b(b),   a(a)
{
}

template<typename T_Data>
Image::Pixel<T_Data>::Pixel(const Image::PixelRef& pRef)
{
    auto* d = static_cast<T_Data*>(pRef.data);
    a = d[pRef.ap];
    b = d[pRef.bp];
    g = d[pRef.gp];
    r = d[pRef.rp];
}

template<typename T_Data>
Image::Pixel<T_Data>& Image::Pixel<T_Data>::operator=(const Image::PixelRef& pRef)
{
    auto* d = static_cast<T_Data*>(pRef.data);
    a = d[pRef.ap];
    b = d[pRef.bp];
    g = d[pRef.gp];
    r = d[pRef.rp];
    return *this;
}

template<typename T_Data>
Image::PixelRef& Image::PixelRef::operator=(const Image::Pixel<T_Data>& p)
{
    auto* d = static_cast<T_Data*>(data);
    d[rp] = p.r;
    d[gp] = p.g;
    d[bp] = p.b;
    d[ap] = p.a;
    return *this;
}


// Image template member functions
template<typename T_Data>
void Image::setPixel(int x, int y, const Image::Pixel<T_Data>& p)
{
    auto* d = static_cast<T_Data*>(_data);
    uint64_t pos = (y*_width + x)*nChannels(_dataFormat);
    d[pos+_interleave[0]] = p.r;
    d[pos+_interleave[1]] = p.g;
    d[pos+_interleave[2]] = p.b;
    d[pos+_interleave[3]] = p.a;
}

template <typename T_Data>
T_Data* Image::data()
{
    // Check for data type
    assert(_dataType == dataTypeEnum<T_Data>());

    return static_cast<T_Data*>(_data);
}

template <typename T_Data>
const T_Data* Image::data() const noexcept
{
    // Check for data type
    assert(_dataType == dataTypeEnum<T_Data>());

    return static_cast<T_Data*>(_data);
}

template <typename T_Data>
void Image::dataDeleter(void* data) {
    if (data != nullptr)
        delete[] static_cast<T_Data*>(data);
}
