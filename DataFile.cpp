#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
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

    return new DuoDuo::DataFile(folder, name_to_file_name(name), fd);
}

std::string DataFile::name_to_file_name(const std::string& name)
{
    return name + ".data";
}

DataFile::DataFile(const std::string& folder, const std::string& filename, int fd)
    : m_folder(folder)
    , m_filename(filename)
    , m_fd(fd)
{
    printf("\ncreate DataFile: %s %s\n", m_folder.c_str(), m_filename.c_str());
}

std::string DataFile::get_full_path_file_name(void) const
{
    return m_folder + m_filename;
}

f_offset DataFile::add_data(const std::string& value)
{
    // todo: lock whole file when multi app mode.

    // moving file pos to the end.
    f_offset lseek_ret = lseek(m_fd, 0, SEEK_END);
    assert_check(lseek_ret >= 0, "DataFile::add_data");

    // write data.
    ssize_t write_ret = write(m_fd, value.c_str(), value.length());
    if (write_ret == -1)
        error_log("Write data file failed: %s -- %s\n", m_filename.c_str(), strerror(errno));
    return lseek_ret;
}

std::string DataFile::get_data(f_offset pos, size_t size) const
{
    assert_check(pos >= 0, 'DataFile::get_data');
    assert_check(size > 0, 'DataFile::get_data');

    // checking file size.
    {
        struct stat file_stat;
        int fstat_ret = fstat(m_fd, &file_stat);
        if (fstat_ret == -1)
            error_log("Get file stat failed: %s -- %s\n", m_filename.c_str(), strerror(errno));
        if (pos + size > file_stat.st_size)
        {
            char msg[1024] = "";
            snprintf(msg, sizeof(msg), "%s file damaged: try to read the invalid pos of file!", m_filename.c_str());
            error_exit(msg);
        }
    }

    // todo: lock [pos, size] in file
    {
    }

    // read it.
    {
        char value[size];
        ssize_t pread_ret = pread(m_fd, value, size, pos);
        if (pread_ret == -1)
        {
            char msg[1024] = "";
            snprintf(msg, sizeof(msg), "%s file damaged: Read file fail! pos:%lld, size:%lu", m_filename.c_str(), pos, size);
            error_exit(msg);
        }
        return std::string(value, size);
    }
}
