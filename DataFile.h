#ifndef DUO_DATA_FILE_H_
#define DUO_DATA_FILE_H_

#include <string>
#include "typedef.h"

namespace DuoDuo
{
    class DataFile
    {
    public:
        static DataFile* create(const std::string& folder, const std::string& name);

        DataFile(const std::string& folder, const std::string& filename, int fd);
        f_offset add_data(const std::string& value);
        //f_offset change_data(f_offset begin_pos, const std::string& value);

        std::string get_full_path_file_name(void) const;

    private:
        static std::string name_to_file_name(const std::string& name);

    private:
        std::string m_folder;
        std::string m_filename;
        int m_fd;
    };
}

#endif
