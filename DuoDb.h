#include <string>
#include <map>

#include "DbImp.h"

// the interface of DuoDuo.
namespace DuoDuo
{
    class DB
    {
    public:
        DB(const std::string& name);
        DB(const char* name);

        static unsigned int config_db_folder(const std::string& full_path_folder);

        const std::string& get(const std::string& value);

        // First version of DuoDB will append file to store a value when changing value of a exist key.
        //   And it do not reuse the old position in file.
        unsigned int set(const std::string& key, const std::string& value);

        //unsigned int add(const std::string& key, const std::string& value);
        //void save(int strategy);
    private:
        static std::map<std::string /*name*/, DBImp> m_dbs;
        std::string m_name;
    };
}
