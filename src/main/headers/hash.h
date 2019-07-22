#pragma once

#include <cstdint>

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
        head = 0;
        tail = 0;
        size = 0;
    }

protected:
    uint8_t window[N];
    int head;
    int tail;
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
            retval = hash.update(in, RollingHash<N>::window[RollingHash<N>::tail], N);
            RollingHash<N>::tail = (RollingHash<N>::tail + 1) % N;
        }

        RollingHash<N>::window[RollingHash<N>::head] = in;
        RollingHash<N>::head = (RollingHash<N>::head + 1) % N;

        return retval;
    }

    void clear() {
        RollingHash<N>::clear();
        hash.reset();
    }

private:
    Adler32 hash;
};

} // namespace rstore
