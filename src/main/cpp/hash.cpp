
#include "hash.h"

namespace rstore
{


#define ADLER32BASE 65521

Adler32::Adler32()
{
    reset();
}

uint32_t Adler32::eat(uint8_t in)
{
    uint16_t sum2 = (hashvalue >> 16) & 0xFFFF;
    uint16_t sum1 = hashvalue & 0xFFFF;

    sum1 = (sum1 + in) % ADLER32BASE;
    sum2 = (sum2 + sum1) % ADLER32BASE;

    hashvalue = (sum2 << 16) | sum1;
    return hashvalue;
}

uint32_t Adler32::update(uint8_t in, uint8_t out, uint16_t len)
{
    uint16_t sum2 = (hashvalue >> 16) & 0xFFFF;
    uint16_t sum1 = hashvalue & 0xFFFF;

    sum1 = (sum1 - out + in) % ADLER32BASE;
    sum2 = (sum2 - (len+1) * out + sum1) % ADLER32BASE;
    hashvalue = (sum2 << 16) | sum1;
    return hashvalue;
}

void Adler32::reset()
{
    hashvalue = 1;
}

    
} // namespace rstore
