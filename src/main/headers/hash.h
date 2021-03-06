#pragma once

#include <cstdint>
#include <stddef.h>

namespace rstore
{

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
};

template<int N>
class RollingHash {
public:

    RollingHash()
    {
        clear();
    }

    void clear() {
        ptr = 0;
        size = 0;
    }

protected:
    uint8_t window[N];
    int ptr;
    int size;
};

template<int N>
class Adler32RollingHash : public RollingHash<N> {
public:

    uint32_t update(uint8_t in) {
        uint32_t retval;

        if(RollingHash<N>::size < N) {
            RollingHash<N>::size++;
            retval = hash.update(in);
        } else {
            retval = hash.update(in, RollingHash<N>::window[RollingHash<N>::ptr], N);
        }

        RollingHash<N>::window[RollingHash<N>::ptr] = in;
        RollingHash<N>::ptr = (RollingHash<N>::ptr + 1) % N;

        return retval;
    }

    uint32_t update(void* buf, size_t offset, size_t len) {
        uint32_t hashcode;
        uint8_t* data = (uint8_t*) buf;
        for(size_t i=0;i<len;i++) {
            hashcode = update(data[i + offset]);
        }
        return hashcode;
    }

    void clear() {
        RollingHash<N>::clear();
        hash.reset();
    }

private:
    Adler32 hash;
};

} // namespace rstore
