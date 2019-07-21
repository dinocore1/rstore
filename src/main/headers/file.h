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

    std::string path;
};

} // namespace rstore