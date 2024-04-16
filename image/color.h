#pragma once

#include <cstdint>

struct Color {
public:
    Color() = default;

    Color(uint8_t blue, uint8_t green, uint8_t red) {
        SetVals(blue, green, red);
    };

    uint8_t blue = 0;
    uint8_t green = 0;
    uint8_t red = 0;

    void SetVals(uint8_t in_blue, uint8_t in_green, uint8_t in_red) {
        blue = in_blue;
        green = in_green;
        red = in_red;
    };
};
