#include <assert.h>

#include "Util.h"
#include "DbImp.h"

using namespace DuoDuo;

DuoDuo::DBImp::DBImp(const std::string& file_folder, const std::string& file_name)
    : m_file_folder(file_folder)
    , m_file_name(file_name)
    , m_p_data_file(NULL)
    , m_p_index_file(NULL)
{
    assert_check(file_folder.size() > 0, "");
    assert_check(file_name.size() > 0, "");

    // making folder end of /
    //
    if (m_file_folder[m_file_folder.size() - 1] != '/')
        m_file_folder.append("/");
}
