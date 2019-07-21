#pragma once

#include <string>

namespace rstore {

class File {
public:

    File();
    File(const File& copy);
    File(const std::string& path);
    File(const File& base, const std::string& path);


    bool exists();
    bool mkdir();
    bool isFile();
    bool isDirectory();

    std::string path;
};

class FPStat {
public:
    FPStat(FILE* = nullptr);
    ~FPStat();

    void close();

    operator FILE* ();
private:
    FILE* fp;
};

} // namespace rstore