#include "Config.h"


DuoDuo::Config DuoDuo::Config::m_Ins;
DuoDuo::Config& DuoDuo::Config::ins(void)
{
    return m_Ins;
}
