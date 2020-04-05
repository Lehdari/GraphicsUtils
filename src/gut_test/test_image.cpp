//
// Project: GraphicsUtils
// File: test_image.cpp
//
// Copyright (c) 2020 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "tests.hpp"
#include <gut_image/Image.hpp>
#include <gut_utils/Stopwatch.hpp>


using namespace gut;


int gut::testImage()
{

    // Test image load, write and RO5
    {
        Image img;
        img.loadFromFile(std::string(RES_PATH)+"images/lenna.png");
        img.writeToFile("output/testImage_lenna1.png");
        img.writeToFile("output/testImage_lenna1.jpg");
        img.writeToFile("output/testImage_lenna1.bmp");
        img.writeToFile("output/testImage_lenna1.tga");
        Image img2(img);
        Image img3(std::move(img2));
        Image img4 = img3;
        Image img5 = std::move(img4);
        img.writeToFile("output/testImage_lenna2.png");
        img3.writeToFile("output/testImage_lenna3.png");
        img5.writeToFile("output/testImage_lenna4.png");
    }

    uint64_t t1, t2, t3;
    // Test direct data pointer access
    {
        Image img(Image::DataFormat::RGBA, Image::DataType::U8);
        img.create(4096, 4096);

        Stopwatch sw;
        sw.start();

        // Output all RGB colors
        auto* p = img.data<uint8_t>();
        for (int y=0; y<4096; ++y) {
            for (int x=0; x<4096; ++x) {
                int i = (y*4096 + x)*4;
                p[i] = x % 256;
                p[i+1] = y % 256;
                p[i+2] = (y/256)*16 + (x/256);
                p[i+3] = 255;
            }
        }

        t1 = sw.stop();
        printf("Direct data access: %llu\n", t1);

        img.writeToFile("output/testImage_direct.png");
    }

    // Test pixel reference access
    {
        Image img(Image::DataFormat::RGBA, Image::DataType::U8);
        img.create(4096, 4096);

        Stopwatch sw;
        sw.start();

        // Output all RGB colors
        for (int y = 0; y < 4096; ++y) {
            for (int x = 0; x < 4096; ++x) {
                img(x, y) = Image::Pixel<uint8_t>(x%256, y%256, (y/256)*16+(x/256), 255);
            }
        }

        t2 = sw.stop();
        printf("Pixel reference:    %llu (", t2);
        printf("%0.4f)\n", ((double)t2/(double)t1)*100.0);

        img.writeToFile("output/testImage_pixelReference.png");
    }

    // Test setPixel access
    {
        Image img(Image::DataFormat::RGBA, Image::DataType::U8);
        img.create(4096, 4096);

        Stopwatch sw;
        sw.start();

        // Output all RGB colors
        for (int y = 0; y < 4096; ++y) {
            for (int x = 0; x < 4096; ++x) {
                img.setPixel(x, y, Image::Pixel<uint8_t>(x%256, y%256, (y/256)*16+(x/256), 255));
            }
        }

        t3 = sw.stop();
        printf("setPixel:           %llu (", t3);
        printf("%0.4f)\n", ((double)t3/(double)t1)*100.0);

        img.writeToFile("output/testImage_setPixel.png");
    }

    return 0;
}
