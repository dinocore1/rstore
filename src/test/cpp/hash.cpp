#include <gtest/gtest.h>

#include "hash.h"

using namespace rstore;

TEST(Adler32, roll) {

    
    uint8_t data[] = {
        'a', 'b', 'c', 'd', 'e', 'f'
    };

    Adler32 hash1, hash2;
    hash1.eat(data[0]);
    hash1.eat(data[1]);
    hash1.eat(data[2]);
    
    uint32_t hash1_bcd = hash1.update(data[3], data[0], 3);

    hash2.eat(data[1]);
    hash2.eat(data[2]);
    uint32_t hash2_bcd = hash2.eat(data[3]);

    ASSERT_FALSE(0 == hash1_bcd);
    ASSERT_EQ(hash1_bcd, hash2_bcd);


}