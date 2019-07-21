
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static void usage()
{
    fprintf(stdout, "RStore \n");
    fprintf(stdout, "Usage: rstore [CMD] [ARGS]\n\n");
    fprintf(stdout, "Commands:\n");
    fprintf(stdout, "  init [path]\tinitialize rstore database in directory\n");
    fprintf(stdout, "  ls [path]\tlist files under [path]\n");
}

static int init(int argc, char** argv)
{
    int c;
    char* dirpath;
    while((c = getopt(argc, argv, "h")) != -1) {
        switch(c) {
            case 'h':
                fprintf(stdout, "init [PATH]\n");
                fprintf(stdout, "  PATH full path to directory\n");
                return -1;
        }
    }

    if(argc - optind != 1) {
        fprintf(stderr, "required path\n");
        return -1;
    }

    dirpath = argv[optind];

    printf("dir path: %s\n", dirpath);
    return 0;
}

int main(int argc, char** argv) {
    
    int c;
    char* cmd;

    while( (c = getopt(argc, argv, "h")) != -1) {
        switch(c) {
            case 'h':
                usage();
                return -1;
        }

    }

    if( argc - optind < 1 ) {
        fprintf( stderr, "required command. See help\n" );
        return -1;
    }

    cmd = argv[optind];

    if( strcmp(cmd, "init") == 0 ) {
        return init(argc-1, &argv[optind]);
    }

    fprintf(stderr, "unknown command: %s\n", cmd);
    return -1;
}