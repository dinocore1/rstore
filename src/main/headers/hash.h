#pragma once

#include <cstdint>

namespace rstore
{


template<int N>
class RollingHash {
public:

    void clear() {
        head = 0;
        tail = 0;
        size = 0;
    }

private:
    uint8_t window[N];
    int head;
    int tail;
    int size;
};


class Adler32 {
public:
    Adler32();

    uint32_t eat(uint8_t in);
    uint32_t update(uint8_t in, uint8_t out, uint16_t len);
    void reset();

private:
    uint32_t hashvalue;

};
    
} // namespace rstore
