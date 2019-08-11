#pragma once

#include <cstddef>
#include <string>
#include "hash.h"
#include "sha256.h"


namespace rstore
{

typedef Adler32RollingHash<16> rollinghash_t;

class BlockChuncker {
public:
typedef void(*write_t)(void* user, void* buf, size_t len);
typedef void(*block_found_t)(const std::string& hash);

void init(void* write_user, write_t, block_found_t);
void write(void* ptr, size_t len);

private:
void* write_user;
write_t write_fn;
block_found_t block_found_fn;
rollinghash_t rolling_hash;
SHA256 secure_hash;


};
    
} // namespace rstore
