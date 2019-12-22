
#include <unistd.h>

#include <iostream>

static
void usage()
{
    std::cout << "Usage: rstore -i [" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    int opt;
    char* input_file;

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

    

    return 0;
}