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

    /**
     * Feed new data to hash.
     */
    uint32_t update(uint8_t in);

    /**
     * removes the given byte that was fed to the hash len bytes ago.
     */
    uint32_t remove(uint8_t out, uint16_t len);
    
    /**
     * add new data to hash and remove old data fed len bytes ago.
     */
    uint32_t update(uint8_t in, uint8_t out, uint16_t len);
    
    
    void reset();

private:
    uint32_t a, b;
    uint32_t hashvalue;

};
    
} // namespace rstore
