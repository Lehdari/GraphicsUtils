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


#include <string>
#include <cassert>
#include <cstdint>


namespace gut {

    /** @brief  Image class for generic 2D image data storage and I/O
     */
    class Image {
    public:
        /** @brief  Supported image pixel data formats, specifies number and order of channels
         */
        enum class DataFormat {
            GRAY,
            RGB,
            RGBA
        };

        /** @brief  Supported image data types
         *  @note   Matching value type is to be used in template interfaces
         */
        enum class DataType {
            INVALID,
            U8,     // uint8_t
            U16,    // uint16_t
            F32     // float
        };

        inline constexpr static int nChannels(DataFormat dataFormat);

        template <typename T_Data>
        inline constexpr static DataType dataTypeEnum();

        struct PixelRef;

        /** @brief  Pixel struct to be used to modify image data
         *  @tparam T_Data  Data type of the pixel; must match the Image data type
         *  @note   Most pixel operations do not involve safety checks to enable maximum performance
         */
        template <typename T_Data>
        struct Pixel {
            T_Data  r;
            T_Data  g;
            T_Data  b;
            T_Data  a;

            /** @brief  Construct a pixel object
             *  @param  r   Red channel value
             *  @param  g   Green channel value
             *  @param  b   Blue channel value
             *  @param  a   Alpha channel value
             */
            Pixel(T_Data r, T_Data g, T_Data b, T_Data a);

            Pixel(const PixelRef& pRef);
            Pixel<T_Data>& operator=(const PixelRef& pRef);
        };

        /** @brief  Pixel reference struct, used as a proxy object when accessing image data
         */
        struct PixelRef {
        private:
            void*       data;
            uint64_t    stride;
            uint64_t    rp;
            uint64_t    gp;
            uint64_t    bp;
            uint64_t    ap;

            friend class Image;
            template <typename T_Data>
            friend class Pixel;

            PixelRef(void* data, uint64_t stride, uint64_t rp, uint64_t gp, uint64_t bp, uint64_t ap);

        public:
            template <typename T_Data>
            PixelRef& operator=(const Pixel<T_Data>& p);

            PixelRef& operator++();
            PixelRef operator++(int);
        };

        /** @brief  Construct an Image object
         *  @param  dataFormat  Pixel data format (number and order of channels)
         *  @param  dataType    Pixel data type (precision)
         */
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

        /** @brief  Get pixel data format of the image
         *  @return Pixel data format of the image
         */
        DataFormat dataFormat() const noexcept;

        /** @brief  Get data type of the image
         *  @return Data type of the image
         */
        DataType dataType() const noexcept;

        /** @brief  Get width of the image
         *  @return Width of the image in pixels
         */
        int width() const noexcept;
        /** @brief  Get  of the image
         *  @return Height of the image in pixels
         */
        int height() const noexcept;

        /** @brief  Access a pixel at location
         *  @param  x   x-coordinate of the pixel to be accessed
         *  @param  y   y-coordinate of the pixel to be accessed
         *  @return Pixel at given location
         *  @note   This operator does not perform boundary checks to allow for maximum performance
         */
        PixelRef operator()(int x, int y);

        /** @brief  Set a pixel at location
         *  @param  x   x-coordinate of the pixel to be set
         *  @param  y   y-coordinate of the pixel to be set
         *  @note   This operator does not perform boundary or type checks to allow for maximum performance
         */
        template <typename T_Data>
        void setPixel(int x, int y, const Pixel<T_Data>& p);

        /** @brief  Access the raw data array
         *  @tparam T_Data  Image data type (uint8_t, uint16_t or float)
         *  @return Read-only pointer to raw image data
         *  @note   The function performs type checking (use dataType() to check)
         *  @note   Data is arranged in row-major order with pixels ordered as specified by the pixel data format
         */
        template <typename T_Data>
        T_Data* data();

        /** @brief  Access the raw data array
         *  @tparam T_Data  Image data type (uint8_t, uint16_t or float)
         *  @return Read-only pointer to raw image data
         *  @note   The function performs type checking (use dataType() to check)
         *  @note   Data is arranged in row-major order with pixels ordered as specified by the pixel data format
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
        int         _interleave[4]; // interleaved positions for R, G, B, A channels

        template <typename T_Data>
        static void dataDeleter(void* data);
    };

    #include "Image.inl"

} // namespace gut


#endif //GRAPHICSUTILS_IMAGE_HPP
