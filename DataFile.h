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

        std::string get_data(f_offset pos, size_t size) const;

        // add data to the end of file. return beginning pos of data in data file.
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
