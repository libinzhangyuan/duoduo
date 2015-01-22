#include "../Config.h"

#include <check_function.h>
#include "BlockStructure_Normal.h"

using namespace DuoDuo;

namespace DuoDuo
{
    const size_t BCOUNT_KEY_LEN = 2;
    const size_t BCOUNT_DATA_POS = 2;
    char KEY_CHECK_NUM = char(0xF1);

    const size_t BCOUNT_DATA_LEN = 2;
    const char DATA_CHECK_NUM = char(0xF2);
}

BlockStructure_Normal::BlockStructure_Normal(std::string& block)
    : BlockStructure(block, BlockStructure::eBlockType_Normal)
    , m_KeySectionSize(0)
    , m_DataSectionSize(0)
{
    size_t ratio = Config::Ins().normal_block_key_ratio_to_16();
    size_t blockSize = block.size();
    m_KeySectionSize = ( (blockSize >> 4) * ratio - HeadSize() );
    m_DataSectionSize = ( (blockSize >> 4) * (16 - ratio) );
}

bool BlockStructure_Normal::IsEnoughForData(const std::string& block, const std::string& key, const std::string& value) const
{
    return false;
}

void BlockStructure_Normal::AppendData(std::string& block, const std::string& key, const std::string& value)
{
}

size_t BlockStructure_Normal::KeySectionSize(void) const
{
    return m_KeySectionSize;
}

size_t BlockStructure_Normal::DataSectionSize(void) const
{
    return m_DataSectionSize;
}

size_t BlockStructure_Normal::GetKeyNeedLen(const std::string& key)
{
    assert_check(key.size() > 0, "BlockStructure_Normal::GetKeyNeedLen");
    return BCOUNT_KEY_LEN + BCOUNT_DATA_POS + sizeof(KEY_CHECK_NUM) + key.size();
}

size_t BlockStructure_Normal::GetValueNeedLen(const std::string& value)
{
    return BCOUNT_DATA_LEN + sizeof(DATA_CHECK_NUM) + value.size();
}
