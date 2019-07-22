#include <gtest/gtest.h>

#include "hash.h"

using namespace rstore;

TEST(Adler32, remove) {

    
    uint8_t data[] = {
        'a', 'b', 'c', 'd', 'e', 'f'
    };

    Adler32 hash1, hash2;

    hash1.update(data[0]);
    hash1.update(data[1]);
    hash1.update(data[2]);
    hash1.remove(data[0], 3);
    uint32_t hash1_bcd = hash1.update(data[3]);

    hash2.update(data[1]);
    hash2.update(data[2]);
    uint32_t hash2_bcd = hash2.update(data[3]);

    ASSERT_FALSE(0 == hash1_bcd);
    ASSERT_EQ(hash1_bcd, hash2_bcd);
}

TEST(Adler32, rollupdate) {

    
    uint8_t data[] = {
        'a', 'b', 'c', 'd', 'e', 'f'
    };

    Adler32 hash1, hash2;

    hash1.update(data[0]);
    hash1.update(data[1]);
    hash1.update(data[2]);
    uint32_t hash1_bcd = hash1.update(data[3], data[0], 3);

    hash2.update(data[1]);
    hash2.update(data[2]);
    uint32_t hash2_bcd = hash2.update(data[3]);

    ASSERT_FALSE(0 == hash1_bcd);
    ASSERT_EQ(hash1_bcd, hash2_bcd);
}

TEST(Adler32Rollinghash, update) {
    Adler32RollingHash<3> hash1;
    Adler32 hash2;

    uint8_t data[] = {
        'a', 'b', 'c', 'd', 'e', 'f'
    };

    hash1.update(data[0]);
    hash1.update(data[1]);
    hash1.update(data[2]);
    uint32_t hash1_bcd = hash1.update(data[3]);

    hash2.update(data[1]);
    hash2.update(data[2]);
    uint32_t hash2_bcd = hash2.update(data[3]);

    ASSERT_EQ(hash2_bcd, hash1_bcd);
}