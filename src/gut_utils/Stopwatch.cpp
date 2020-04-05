//
// Project: GraphicsUtils
// File: Stopwatch.cpp
//
// Copyright (c) 2020 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include <cstdio>
#include "Stopwatch.hpp"


using namespace gut;


Stopwatch::Stopwatch(uint64_t nCalib) :
    _startTimeStamp (0),
    _calibDiff      (0)
{
    uint64_t sum = 0;
    for (uint64_t i=0; i<nCalib; ++i) {
        start();
        uint64_t dt = stop();
        sum += dt;
    }

    _calibDiff = sum/nCalib;
}
