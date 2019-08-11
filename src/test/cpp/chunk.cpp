#include <gtest/gtest.h>

#include "blockchunk.h"

using namespace rstore;

TEST(BlockChuncker, block_divider) {

    uint8_t buf[] = {0x66, 0x66, 0x66, 0x66, 0x66, 0x66 };
    uint8_t buf1[] = {0x6c, 0x74, 0x65, 0x72, 0x2f, 0x46, 0x6c, 
    0x61, 0x74, 0x65, 0x44, 0x65, 0x63, 0x6f, 0x64, 0x65, 0x2f};

    char output[32];
    size_t output_i = 0;

    BlockChuncker chunker;

    auto onwrite = [&](void* buf, size_t len) {
        printf("onwrite %d\n", len);
        memcpy(&output[output_i], buf, len);
        output_i += len;
    };

    auto onblock = [](const std::string& hash){
        printf("onblock\n");
    };

    chunker.init(onwrite, onblock);
    //chunker.write(buf, 6);
    chunker.write(buf1, 17);
    chunker.write(buf, 6);
    chunker.write(buf, 6);
    chunker.finish();


}
