#include "../Config.h"

#include <check_function.h>
#include "BlockStructure_Normal.h"

using namespace DuoDuo;

BlockStructure_Normal::BlockStructure_Normal(size_t blockSize)
    : BlockStructure(BlockStructure::eBlockType_Normal, blockSize)
    , m_KeySectionSize(0)
    , m_DataSectionSize(0)
{
    size_t ratio = Config::Ins().normal_block_key_ratio_to_16();
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
