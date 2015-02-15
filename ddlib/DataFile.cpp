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
#include "block/DataBlock.h"
#include "block/BlockStructure_Normal.h"

using namespace DuoDuo;


namespace DuoDuo
{
    bool DataFile::CachedKeyValueContainer::HasSmallData(void) const
    {
        BlockStructure_Normal::StructCalc normalCalc(m_BlockSize);
        for (key_value_map_t::const_iterator iter = m_CachedKeyValues.begin(); iter != m_CachedKeyValues.end(); ++iter)
        {
            const std::string& key = iter->first;
            const std::string& value = iter->second;
            if (normalCalc.IsNormalData(key, value) == true)
                return true;
        }
        return false;
    }

    bool DataFile::CachedKeyValueContainer::HasBigData(void) const
    {
        BlockStructure_Normal::StructCalc normalCalc(m_BlockSize);
        for (key_value_map_t::const_iterator iter = m_CachedKeyValues.begin(); iter != m_CachedKeyValues.end(); ++iter)
        {
            const std::string& key = iter->first;
            const std::string& value = iter->second;
            if (normalCalc.IsNormalData(key, value) == false)
                return true;
        }
        return false;
    }

    std::vector<DataBlock> DataFile::CachedKeyValueContainer::PopBigBlock(void)
    {
        BlockStructure_Normal::StructCalc normalCalc(m_BlockSize);
        for (key_value_map_t::const_iterator iter = m_CachedKeyValues.begin(); iter != m_CachedKeyValues.end(); ++iter)
        {
            const std::string& key = iter->first;
            const std::string& value = iter->second;
            if (normalCalc.IsNormalData(key, value) == false)
            {
                m_CachedKeyValues.erase(iter);
                return DataBlock::CreateBlockWithBigData(m_BlockSize, key, value);
            }
        }
        return std::vector<DataBlock>();
    }
}

namespace DuoDuo
{
    DataFile::LastUnfilledNormalBlock::LastUnfilledNormalBlock(size_t block_size)
        : m_BlockIndex(-1)
    {
        m_NormalBlock = DataBlock::EmptyNormalBlock(block_size);
    }

    DataFile::LastUnfilledNormalBlock::~LastUnfilledNormalBlock()
    {
    }

}

DataFile* DataFile::Create(const std::string& folder, const std::string& name, size_t block_size)
{
    assert_check(folder.size() > 0, "DataFile::create");
    assert_check(folder[folder.size() - 1] == '/', "DataFile::create");
    assert_check(name.size() > 0, "DataFile::create");
    assert_check(block_size > 0, "DataFile::create");
    assert_check( ((block_size >> 4) << 4) == block_size, "DataFile::create" );

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

    return new DuoDuo::DataFile(folder, name_to_file_name(name), fd, block_size);
}

DataFile::DataFile(const std::string& folder, const std::string& filename, int fd, size_t block_size)
    : m_Folder(folder)
    , m_Filename(filename)
    , m_Fd(fd)
    , m_BlockSize(block_size)
    , m_LastUnfilledNormalBlock(block_size)
    , m_CachedKeyValues(block_size)
{
    printf("\ncreate DataFile: %s %s\n", m_Folder.c_str(), m_Filename.c_str());
}

void DataFile::AddData(const std::string& key, const std::string& value)
{
    m_CachedKeyValues.AddData(key, value);
}

// Save() will saving all unsaved data to file.
// the small data will add to last unfilled normalBlock first.
// And it will reset the last normalBlock.
void DataFile::Save(void)
{
    //  if there are big data only
    //      saving all big data && clean those data.
    //      return
    //  end
    //
    //  // have small data
    //  if there is lastUnfilledNormalBlock
    //      old_lastUnfilledNormalBlock = lastUnfilledNormalBlock
    //      data_PlaceIntoLastUnfilledNormalBlock = find(key that is not accur after lastUnfilledNormalBlock && the rest of block can pack it && the same key in lastUnfilledNormalBlock is prefer)
    //      if data_PlaceIntoLastUnfilledNormalBlock is not empty
    //          removing those small data from cachedkeyvalues
    //          place those small data into lastUnfilledNormalBlock and save it. // (do not change the pos of the old data in lastUnfilledNormalBlock)
    //          notify the index of old data in lastUnfilledNormalBlock changed
    //      end
    //      if there is small data in cachedkeyvalues still
    //          clean lastUnfilledNormalBlock
    //      end
    //  end
    //
    //  saving all big data && clean those data.
    //  if there is small data in cachedkeyvalues still
    //      saving all small data to the end && clean those data.
    //      set last unfilled normalblock with new situation.
    //  end
}

void DataFile::SaveFilledBlock(void)
{
    //  if there are big data only
    //      saving all big data && clean those data.
    //      return
    //  end
    //
    //  // have small data
    //  if there is lastUnfilledNormalBlock
    //      old_lastUnfilledNormalBlock = lastUnfilledNormalBlock
    //      data_PlaceIntoLastUnfilledNormalBlock = find(key that is not accur after lastUnfilledNormalBlock && the rest of block can pack it)
    //      if data_PlaceIntoLastUnfilledNormalBlock is not empty
    //          removing those small data from cachedkeyvalues
    //          place those small data into lastUnfilledNormalBlock and save it. // (do not change the pos of the old data in lastUnfilledNormalBlock)
    //          notify the index of old data in lastUnfilledNormalBlock changed
    //      end
    //      if there is small data in cachedkeyvalues still
    //          clean lastUnfilledNormalBlock
    //      end
    //  end
    //
    //  saving all big data && clean those data.
    //  if there is small data in cachedkeyvalues still
    //      saving filled block with small data && clean those data && left the rest data in cachedkeyvalues
    //  end
}

std::string DataFile::name_to_file_name(const std::string& name)
{
    return name + ".data";
}

std::string DataFile::GetFullPathFileName(void) const
{
    return m_Folder + m_Filename;
}
