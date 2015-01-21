#include "Config.h"

DuoDuo::Config DuoDuo::Config::m_Ins;
DuoDuo::Config& DuoDuo::Config::Ins(void)
{
    return m_Ins;
}
