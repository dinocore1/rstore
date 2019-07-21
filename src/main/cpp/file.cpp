
#include "file.h"

#include <sys/stat.h>

namespace rstore
{

File::File()
{}

File::File(const File& copy)
 : path(copy.path)
{}

File::File(const std::string& path)
 : path(path)
{}

File::File(const File& base, const std::string& path)
 : path( base.path + "/" + path )
{}

bool File::mkdir()
{
    return ::mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0;
}

bool File::exists()
{
    struct stat sb;
    return stat(path.c_str(), &sb) == 0;
}

} // namespace rstore
