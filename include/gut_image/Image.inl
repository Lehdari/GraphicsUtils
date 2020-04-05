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


template <typename T_Data>
T_Data* Image::data()
{
    // Check for data type
    assert(_dataType == dataTypeEnum<T_Data>());

    return static_cast<std::vector<T_Data>*>(_data)->data();
}

template <typename T_Data>
const T_Data* Image::data() const noexcept
{
    // Check for data type
    assert(_dataType == dataTypeEnum<T_Data>());

    return static_cast<std::vector<T_Data>*>(_data)->data();
}

template <typename T_Data>
void Image::dataDeleter(void* data) {
    if (data != nullptr)
        delete static_cast<std::vector<T_Data>*>(data);
}
