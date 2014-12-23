#ifndef DUO_DB_IMP_H_
#define DUO_DB_IMP_H_

#include <string>
#include "DataFile.h"
#include "IndexFile.h"

namespace DuoDuo
{
    class DBImp
    {
    public:
        DBImp(const std::string& file_folder, const std::string& file_name);

        const std::string& get_folder(void) const {return m_file_folder;};


    private:
        std::string m_file_folder;
        std::string m_file_name;
        DataFile* m_p_data_file;
        IndexFile* m_p_index_file;
    };
}


#endif
