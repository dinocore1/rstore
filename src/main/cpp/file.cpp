
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

bool File::isFile()
{
   struct stat sb;
   stat(path.c_str(), &sb) == 0 &&
   sb.st_mode & S_IFMT == S_IFREG; 
}

bool File::isDirectory()
{
   struct stat sb;
   stat(path.c_str(), &sb) == 0 &&
   sb.st_mode & S_IFMT == S_IFDIR;
}

FPStat::FPStat(FILE* fp)
 : fp(fp)
{}

FPStat::~FPStat()
{
    close();
}

FPStat& FPStat::operator= (FILE* rhs)
{
    close();
    fp = rhs;
    return *this;
}

void FPStat::close()
{
    if(fp) {
        ::fclose(fp);
        fp = nullptr;
    }
}

FPStat::operator FILE*() {
    return fp;
}

} // namespace rstore
