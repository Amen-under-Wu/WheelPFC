//
// Created by fyh on 2024/5/24.
//

#ifndef WHEELPFC_CART_HPP
#define WHEELPFC_CART_HPP

#include <array>
#include <cstdint>

class WheelCartridge {
public:
    std::array<uint8_t, 128> tiles_data;
    std::array<uint8_t, 128> sprites_data;
    std::array<uint8_t, 240*136> map_data;

    virtual void init() {}
    virtual void update() {}
    virtual void scanline(int row) {}
    virtual void overlay() {}
};

#endif //WHEELPFC_CART_HPP
