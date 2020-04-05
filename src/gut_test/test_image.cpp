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

    uint64_t t = sw.stop();
    printf("nCycles: %llu\n", t);

    img.writeToFile("test.png");

    return 0;
}
