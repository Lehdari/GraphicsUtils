//
// Project: GraphicsUtils
// File: Image.hpp
//
// Copyright (c) 2020 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#ifndef GRAPHICSUTILS_IMAGE_HPP
#define GRAPHICSUTILS_IMAGE_HPP


#include <vector>
#include <string>
#include <cassert>


namespace gut {

    class Image {
    public:
        enum class DataFormat {
            GRAY,
            RGB,
            RGBA
        };

        enum class DataType {
            INVALID,
            U8,
            U16,
            F32
        };

        constexpr static int nChannels(DataFormat dataFormat);

        template <typename T_Data>
        constexpr static DataType dataTypeEnum();

        explicit Image(
            DataFormat dataFormat   = DataFormat::RGB,
            DataType dataType       = DataType::U8);

        Image(const Image& other);
        Image(Image&& other) noexcept;
        Image& operator=(const Image& other);
        Image& operator=(Image&& other) noexcept;
        ~Image();

        /** @brief Create an empty image
         *  @param width    Width of the image
         *  @param height   Height of the image
         */
        void create(int width, int height);

        /** @brief Load image from a file
         *  @param fileName Name of the file to load the image from
         */
        void loadFromFile(const std::string& fileName);

        /** @brief  Write image to a file
         *  @param  fileName    Name of the file to write the image to
         *  @note   File format is deduced from the extension, supported formats: png, bmp, jpg, tga, hdr
         *  @note   Supported data types for formats:
         *          PNG: U8
         *          BMP: U8
         *          JPG: U8
         *          TGA: U8
         *          HDR: F32
         */
        void writeToFile(const std::string& fileName);

        DataFormat dataFormat() const noexcept;
        DataType dataType() const noexcept;

        int width() const noexcept;
        int height() const noexcept;

        /** @brief  Access the raw data array
         *  @tparam T_Data  Image data type (uint8_t, uint16_t or float)
         *  @return Read-only pointer to raw image data
         *  @note   The function performs type checking (use dataType() to check)
         */
        template <typename T_Data>
        const T_Data* data() const noexcept;

    private:
        DataFormat  _dataFormat;
        DataType    _dataType;

        int         _width;
        int         _height;

        void*       _data;
        void        (*_deleter)(void*);

        template <typename T_Data>
        static void dataDeleter(void* data);
    };

    #include "Image.inl"

} // namespace gut


#endif //GRAPHICSUTILS_IMAGE_HPP
