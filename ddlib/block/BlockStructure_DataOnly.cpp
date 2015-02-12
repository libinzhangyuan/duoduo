#include "BlockStructure_DataOnly.h"

#include <check_function.h>

#include "../def.h"

using namespace DuoDuo;

namespace DuoDuo
{
    size_t BlockStructure_DataOnly::StructCalc::HeadSize(void) const
    {
        return DD_BLOCK_HEAD_SIZE;
    }

    size_t BlockStructure_DataOnly::StructCalc::MaxValueLen(void) const
    {
        return m_BlockSize - DD_BLOCK_HEAD_SIZE;
    }

    size_t BlockStructure_DataOnly::StructCalc::BlockNeedCount(const size_t data_size_need_store_in_dataonly) const
    {
        size_t count = 0;
        size_t max_value_len = MaxValueLen();
        long long data_size = data_size_need_store_in_dataonly;
        while (data_size > 0)
        {
            count++;
            data_size -= max_value_len;
        }
        return count;
    }
}


void BlockStructure_DataOnly::LoadFromBlock(void)
{
}

bool BlockStructure_DataOnly::IsEnoughForData(const std::string& key, const std::string& value) const
{
    assert_check(false, "BlockStructure_DataOnly::IsEnoughForData:  should not call this function!");
    return false;
}

void BlockStructure_DataOnly::AddData(const std::string& /*key*/, const std::string& value)
{
    const size_t max_value_size_in_block = m_StructCalc.MaxValueLen();
    if (value.size() > max_value_size_in_block)
        m_ValueStoredInBlock = value.substr(0, max_value_size_in_block);
    else
        m_ValueStoredInBlock = value;
}
