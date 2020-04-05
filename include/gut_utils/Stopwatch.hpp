//
// Project: GraphicsUtils
// File: Stopwatch.hpp
//
// Copyright (c) 2020 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#ifndef GRAPHICSUTILS_STOPWATCH_HPP
#define GRAPHICSUTILS_STOPWATCH_HPP


#include <cstdint>
#ifdef _MSC_VER
# include <intrin.h>
#else
# include <x86intrin.h>
#endif


namespace gut {

    class Stopwatch {
    public:
        /** @brief  Construct a Stopwatch object
         *  @param  nCalib  Number of calibration iterations
         */
        Stopwatch(uint64_t nCalib = 100000);

        /** @brief  Start the stopwatch
         */
        inline void start();

        /** @brief  Stop the stopwatch
         *  @return Number of CPU clock cycles elapsed after start() call
         */
        inline uint64_t stop();

    private:
        int64_t    _startTimeStamp;
        int64_t    _calibDiff;
    };


    inline void Stopwatch::start()
    {
        _startTimeStamp = _rdtsc();
    }

    inline uint64_t Stopwatch::stop()
    {
        int64_t r = (int64_t)_rdtsc()-_startTimeStamp-_calibDiff;
        return r < 0 ? 0 : (uint64_t)r;
    }

}


#endif //GRAPHICSUTILS_STOPWATCH_HPP
