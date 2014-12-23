#include <string>

namespace DuoDuo
{
    class Config
    {
    public:
        Config() {}
        static Config& ins(void);

        const std::string& get_db_folder(void) const {return m_folder;}
        void set_db_folder(const std::string& full_path_folder) {m_folder = full_path_folder;}

    private:
        static Config m_Ins;

        std::string m_folder;
    };
}
