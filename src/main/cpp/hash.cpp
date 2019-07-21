
#include "hash.h"

namespace rstore
{


#define ADLER32BASE 65521

Adler32::Adler32()
{
    reset();
}

uint32_t Adler32::update(uint8_t in)
{
    a = (a + in) % ADLER32BASE;
    b = (b + a) % ADLER32BASE;

    hashvalue = (b << 16) | a;
    return hashvalue;
}

uint32_t Adler32::remove(uint8_t out, uint16_t len)
{
    a = (a + ADLER32BASE - out) % ADLER32BASE;
    b = ((b - ADLER32BASE - 1) + (ADLER32BASE - len)*out) % ADLER32BASE;
    hashvalue = (b << 16) | a;
    return hashvalue;
}

void Adler32::reset()
{
    a = 1;
    b = 0;
    hashvalue = (b << 16) | a;
}

    
} // namespace rstore
