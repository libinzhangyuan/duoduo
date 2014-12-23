#include "DuoDb.h"
#include "Config.h"

using namespace DuoDuo;

static const std::string static_null_value;

unsigned int DuoDuo::DB::config_db_folder(const std::string& full_path_folder)
{
    DuoDuo::Config::ins().set_db_folder(full_path_folder);
    return 0;
}

DuoDuo::DB::DB(const std::string& name)
    : m_name(name)
{
}

DuoDuo::DB::DB(const char* name)
    : m_name(name)
{
}

const std::string& DuoDuo::DB::get(const std::string& name)
{
    return static_null_value;
}

unsigned int DuoDuo::DB::set(const std::string& key, const std::string& value)
{
    return 0;
}
