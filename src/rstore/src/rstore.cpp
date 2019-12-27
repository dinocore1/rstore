
#include "rollsum.h"
#include "blake2.h"

#include <unistd.h>

#include <sys/stat.h>

#include <iostream>
#include <cstring>
#include <string>

#define BUF_SIZE (1024 * 1024)
#define ROLL_WINDOW 31
#define CRYPTO_SIZE 40

uint8_t chunk_buffer[BUF_SIZE];

static
void usage()
{
    std::cout << "Usage: rstore -i [" << std::endl;
    std::cout << std::endl;
}

static
void rs_hexify(char *to_buf, void const *from, int from_len)
{
    static const char hex_chars[] = "0123456789abcdef";
    unsigned char const *from_buf = (unsigned char const *)from;

    while (from_len-- > 0) {
        *(to_buf++) = hex_chars[((*from_buf) >> 4) & 0xf];
        *(to_buf++) = hex_chars[(*from_buf) & 0xf];
        from_buf++;
    }

    *to_buf = 0;
}

static
int write_chunk(uint8_t* buf, int buf_len, char* crypto_hash_hex)
{
    int err;
    FILE* out;
    struct stat fstat_buf;
    std::string dir;
    std::string filename;
    std::string fullpath;
    dir = "data";
    err = mkdir(dir.c_str(), 0755);
    dir += "/";
    dir += std::string(crypto_hash_hex, 2);
    err = mkdir(dir.c_str(), 0755);
    dir += "/";
    dir += std::string(&crypto_hash_hex[2], 2);
    err = mkdir(dir.c_str(), 0755);
    filename = std::string(crypto_hash_hex) + ".dat";

    fullpath = dir + "/" + filename;

    err = stat(fullpath.c_str(), &fstat_buf);
    if(err != 0) {
        out = fopen( fullpath.c_str(), "wb");
        err = fwrite(buf, buf_len, 1, out);
        if(err != 1) {
            printf("error writing chunk\n");
        }
        fclose(out);
    }

    return 0;
}

int main(int argc, char** argv)
{
    int err;
    int opt;
    char* input_file;
    FILE* in;
    Rollsum roll_hash;
    blake2b_state crypto_hash;
    uint8_t crypto_sum[CRYPTO_SIZE];
    char crypto_hex[(CRYPTO_SIZE*2)+1];

    while((opt = getopt(argc, argv, "hi:")) != -1) {
        switch(opt) {
            case 'h':
                usage();
                return -1;

            case 'i':
                input_file = optarg;
                break;
        }
    }

    if( strcmp(input_file, "-") == 0 ) {
        in = stdin;
    } else {
        in = fopen(input_file, "rb");
    }

    RollsumInit(&roll_hash);
    blake2b_init(&crypto_hash, CRYPTO_SIZE);

    int head, tail, size;
    uint8_t window[ROLL_WINDOW];
    head = tail = size = 0;

    while( (err = fgetc(in)) != EOF ) {
        if(roll_hash.count < ROLL_WINDOW) {
            RollsumRollin(&roll_hash, err);
            window[head] = err;
            head = (head + 1) % ROLL_WINDOW;
        } else {
            RollsumRotate(&roll_hash, window[tail], err);
            window[head] = err;
            head = (head + 1) % ROLL_WINDOW;
            tail = (tail + 1) % ROLL_WINDOW;
        }

        blake2b_update(&crypto_hash, &err, 1);
        chunk_buffer[size] = err;
        uint32_t checksum = RollsumDigest(&roll_hash);
        if(checksum < 0x00004000 || size >= BUF_SIZE-1) {

            

            blake2b_final(&crypto_hash, crypto_sum, CRYPTO_SIZE);
            rs_hexify(crypto_hex, crypto_sum, CRYPTO_SIZE);

            write_chunk(chunk_buffer, size, crypto_hex);

            printf("%s\n", crypto_hex);


            RollsumInit(&roll_hash);
            blake2b_init(&crypto_hash, CRYPTO_SIZE);
            head = tail = size = 0;
        }

        size++;
    }

    write_chunk(chunk_buffer, size, crypto_hex);

    return 0;
}