#ifndef DUO_CONFIG_H_
#define DUO_CONFIG_H_

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

        int ssd_block_size() const {return 4096;}

        int normal_block_key_ratio_to_16(void) {return 3;}

    private:
        static Config m_Ins;

        std::string m_folder;
    };
}

#endif
