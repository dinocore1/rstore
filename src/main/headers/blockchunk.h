#pragma once

#include <cstddef>
#include <string>
#include <functional>

#include "hash.h"
#include "sha256.h"


namespace rstore
{

typedef Adler32RollingHash<16> rollinghash_t;

class BlockChuncker {
public:
BlockChuncker();

typedef std::function<void(void*, size_t)> write_t;
typedef std::function<void(const std::string&)> block_found_t;

void init(write_t, block_found_t);
void write(void* ptr, size_t len);
void finish();

private:

void getSecureHex(std::string&);

write_t write_fn;
block_found_t block_found_fn;

rollinghash_t rolling_hash;
SHA256 secure_hash;
size_t block_size;


};
    
} // namespace rstore
