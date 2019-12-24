
#include "rollsum.h"

#include <unistd.h>

#include <iostream>

#define BUF_SIZE 4096
#define ROLL_WINDOW 31

static
void usage()
{
    std::cout << "Usage: rstore -i [" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    int err;
    int opt;
    char* input_file;
    FILE* in;
    Rollsum roll_hash;
    uint8_t buf[BUF_SIZE];

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

        size++;

        uint32_t checksum = RollsumDigest(&roll_hash);
        if(size >= 1024 && checksum < 0x00000E8F) {
            printf("found chunk size: %d %08x\n", size, checksum);
            RollsumInit(&roll_hash);
            head = tail = size = 0;
        }

    }



    return 0;
}