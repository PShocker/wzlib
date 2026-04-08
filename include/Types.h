#pragma once

#include <cstdint>
#include <string>
namespace wz {
    
    struct WzNull {
    };

    struct WzSubProp {
    };

    struct WzConvex {
    };

    struct WzUOL {
        std::u16string uol;
    };

    struct WzCanvas {
        int32_t width;
        int32_t height;
        int32_t format;
        int32_t format2;
        bool is_encrypted;
        int32_t size;
        int32_t uncompressed_size;
        size_t offset;

        WzCanvas()
                : width(0), height(0),
                  format(0), format2(0),
                  is_encrypted(false), size(0), uncompressed_size(0),
                  offset(0) {}
    };

    struct WzSound {
        int32_t length;
        int32_t frequency;
        int32_t size;
        size_t offset;

        WzSound() : length(0), frequency(0), size(0), offset(0) {}
    };

    struct WzVec2D {

        int32_t x;
        int32_t y;

        WzVec2D() : x(0), y(0) {}
        WzVec2D(int new_x, int new_y) : x(new_x), y(new_y) {}
    };
}