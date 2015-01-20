#ifndef DUO_DATA_FILE_H_
#define DUO_DATA_FILE_H_

#include <string>
#include "../typedef.h"

class DataBlock;

namespace DuoDuo
{
    class DataFile
    {
    protected:
        class CachedBlock
        {
        public:
            CachedBlock(void) {}
        private:
            //DataBlock* m_pCachedBlock;
        };

    public:
        static DataFile* Create(const std::string& folder, const std::string& name);

        DataFile(const std::string& folder, const std::string& filename, int fd);

        f_offset AddData(const std::string& key, const std::string& value);

        std::string GetFullPathFileName(void) const;

    private:
        static std::string name_to_file_name(const std::string& name);

    private:
        std::string m_Folder;
        std::string m_Filename;
        int m_Fd;
        CachedBlock m_CachedBlock;
    };
}

#endif
