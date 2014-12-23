#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "DataFile.h"
#include "Util.h"

using namespace DuoDuo;

DataFile* DataFile::create(const std::string& folder, const std::string& name)
{
    assert_check(folder.size() > 0, "DataFile::create");
    assert_check(folder[folder.size() - 1] == '/', "DataFile::create");
    assert_check(name.size() > 0, "DataFile::create");

    // check the exist of folder. and trying to create folder
    {
        // folder not exist
        if (access(folder.c_str(), S_IRUSR) == -1)
        {
            if (mkdir(folder.c_str(), S_IRWXU) == -1)
                return NULL;
        }

        // folder exist but access mode doesn't right
        if (access(folder.c_str(), S_IRWXU) == -1)
            return NULL;
    }

    // try to open file || create file if not exist.
    //
    const std::string filename = folder + name_to_file_name(name);
    int fd = open(filename.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        printf("errno: %d %s\n", errno, strerror(errno));
        return NULL;
    }

    return new DuoDuo::DataFile(folder, name, fd);
}

std::string DataFile::name_to_file_name(const std::string& name)
{
    return name + ".data";
}

DuoDuo::DataFile::DataFile(const std::string& folder, const std::string& filename, int fd)
    : m_folder(folder)
    , m_filename(filename)
    , m_fd(fd)
{
}

std::string DataFile::get_full_path_file_name(void) const
{
    return m_folder + m_filename;
}
