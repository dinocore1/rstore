#include "blockchunk.h"

namespace rstore
{

BlockChuncker::BlockChuncker()
 : block_size(0)
{
    secure_hash.init();
}

void BlockChuncker::init(write_t write, block_found_t blockfound)
{
    this->write_fn = write;
    this->block_found_fn = blockfound;
}

void BlockChuncker::write(void* ptr, size_t len)
{
    uint8_t* buf = (uint8_t*)ptr;

    for(size_t i=0;i<len;i++) {
        block_size++;
        if(rolling_hash.update(buf[i]) < 0x5E0) {
            //new block found

            std::string hash_hex;
            secure_hash.update((const uint8_t*) buf, i);
            getSecureHex(hash_hex);

            write_fn(buf, i);
            block_found_fn(hash_hex);

            secure_hash.init();
            rolling_hash.clear();
            buf = &buf[i];
            len = len - i;
            i = 0;
            block_size = 0;
        }
    }

    secure_hash.update((const uint8_t*) buf, len);
    write_fn(buf, len);
}

void BlockChuncker::finish()
{
    if(block_size > 0) {
        std::string hash_hex;
        getSecureHex(hash_hex);
        block_found_fn(hash_hex);
        block_size = 0;
    }
}

void BlockChuncker::getSecureHex(std::string& hash)
{
    unsigned char digest[SHA256::DIGEST_SIZE];
    memset(digest,0,SHA256::DIGEST_SIZE);
 
    secure_hash.final(digest);
 
    char buf[2*SHA256::DIGEST_SIZE+1];
    buf[2*SHA256::DIGEST_SIZE] = 0;
    for (int i = 0; i < SHA256::DIGEST_SIZE; i++) {
        sprintf(buf+i*2, "%02x", digest[i]);
    }

    hash = std::string(buf);
}
    
} // namespace rstore
