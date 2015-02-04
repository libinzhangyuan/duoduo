#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <check_function.h>
#include "DataFile.h"
#include "Util.h"

using namespace DuoDuo;

DataFile* DataFile::Create(const std::string& folder, const std::string& name)
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
            {
                error_log("\n ERROR: create folder fail!\n");
                return NULL;
            }
        }

        // folder exist but access mode doesn't right
        if (access(folder.c_str(), S_IRWXU) == -1)
        {
            error_log("\nERROR: folder exist but access mode doesn't right!\n");
            return NULL;
        }
    }

    // try to open file || create file if not exist.
    //
    const std::string filename = folder + name_to_file_name(name);
    int fd = open(filename.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        error_log("errno: open file fail: %s : %d %s\n", filename.c_str(), errno, strerror(errno));
        return NULL;
    }

    return new DuoDuo::DataFile(folder, name_to_file_name(name), fd);
}

DataFile::DataFile(const std::string& folder, const std::string& filename, int fd)
    : m_Folder(folder)
    , m_Filename(filename)
    , m_Fd(fd)
{
    printf("\ncreate DataFile: %s %s\n", m_Folder.c_str(), m_Filename.c_str());
}


f_offset DataFile::AddData(const std::string& key, const std::string& value)
{
    // if is the big data.
    //   create new blocks and save
    //   remove same key,value in cached_block
    //   return
    //
    // small data
    // cached block enough for new data
    //   append data to it
    //if (m_pCachedBlock != NULL)
    {
    }

    // cached block not enough
    //     save cached block
    //     create new blocks by new data
    //     if last block need cache
    //         cached it
    //         save other new blocks
    //     else
    //         save all new blocks


    return 0;
}

std::string DataFile::name_to_file_name(const std::string& name)
{
    return name + ".data";
}

std::string DataFile::GetFullPathFileName(void) const
{
    return m_Folder + m_Filename;
}
