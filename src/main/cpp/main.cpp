
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "file.h"
#include "blockchunk.h"

using namespace rstore;

static void usage()
{
    fprintf(stdout, "RStore \n");
    fprintf(stdout, "Usage: rstore CMD [ARGS]\n\n");
    fprintf(stdout, "Commands:\n");
    fprintf(stdout, "  init\tinitialize rstore database in directory\n");
    fprintf(stdout, "  ls\tlist files under [path]\n");
    fprintf(stdout, "  push\tcopy data into into rstore\n");
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
    File dir(dirpath);
    dir = File(dir, ".rstore");

    if(dir.exists()) {
        fprintf(stderr, "rstore dir already exists\n");
        return -1;
    }
    
    if(!dir.mkdir()) {
        fprintf(stderr, "could not mkdir: %s\n", dir.path.c_str());
        return -1;
    }

    File files_dir(dir, "files");
    if(!files_dir.mkdir()) {
        return -1;
    }

    File blocks_dir(dir, "blocks");
    if(!blocks_dir.mkdir()) {
        return -1;
    }

    return 0;
}

#define BUF_SIZE 4096

static int push(int argc, char** argv)
{
    int err;
    int c;
    while((c = getopt(argc, argv, "h")) != -1) {
        switch(c) {
            case 'h':
                fprintf(stdout, "push [-r] SRC_PATH DEST_PATH\n");
                fprintf(stdout, "  copy data from SRC_PATH into the rstore and saves it to DEST_PATH.\n");
                return -1;
        }
    }

    File src_file(argv[optind]);
    if(!src_file.exists()) {
        fprintf(stderr, "file does not exist: %s\n", src_file.path.c_str());
        return -1;
    }

    char buf[BUF_SIZE];
    BlockChuncker chuncker;

    

    File dir(".");
    dir = File(dir, ".rstore");

    File tmp_file(dir, "tmp.dat");

    FPStat fpsrc(::fopen(src_file.path.c_str(), "rb"));
    FPStat fptmp(::fopen(tmp_file.path.c_str(), "wb"));

    auto on_write = [&](void* buf, size_t len) {
        ::fwrite(buf, 1, len, fptmp);
    };

    auto on_block_found = [&](const std::string& hash) {

        File block_file(dir, "blocks");
        block_file = File(block_file, hash);

        fptmp.close();
        rename(tmp_file.path.c_str(), block_file.path.c_str());

        fptmp = ::fopen(tmp_file.path.c_str(), "wb");
    };

    chuncker.init(on_write, on_block_found);

    while( (err = ::fread(buf, 1, BUF_SIZE, fpsrc)) > 0 ) {
        chuncker.write(buf, err);
    }

    chuncker.finish();

    return err;
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
    } else if( strcmp(cmd, "push") == 0) {
        return push(argc-1, &argv[optind]);
    }

    fprintf(stderr, "unknown command: %s\n", cmd);
    return -1;
}