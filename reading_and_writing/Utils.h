#ifndef CPP_HSE_UTILS_H
#define CPP_HSE_UTILS_H

#include <iostream>
#include <string>
#include <vector>

namespace image::utils {
    const int FILE_HEADER_SIZE = 14;
    const int INFO_HEADER_SIZE = 40;
    const int PADDING_SIZE = 4;
    const int PIXEL_DATA_SIZE = 3;
    const int HEADER_WIDTH_OFFSET = 4;
    const int HEADER_HEIGHT_OFFSET = 8;
    const int FORMAT_FILE_FIRST_POSITION = 0;
    const int FORMAT_FILE_SECOND_POSITION = 1;
    const int FILE_SIZE_POSITION = 2;
    const int PIX_POSITION = 10;
    const int INFO_HEADER_SIZE_POSITION = 0;
    const int CNT_OF_PLANE = 1;
    const int CNT_OF_PLANE_POSITION = 12;
    const int CNT_OF_BITS_POSITION = 14;
    const int CNT_OF_BITS = 24;

    const std::vector<char> FORMAT_BYTES = {'B', 'M'};
    const std::vector<int> SHIFT_BYTES = {8, 16, 24};
}  // namespace image::utils

#endif  // CPP_HSE_UTILS_H
