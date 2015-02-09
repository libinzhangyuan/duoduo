
#include "BlockStructure.h"
#include "DataBlock.h"
#include "BlockCreater.h"
#include "../Util.h"

using namespace DuoDuo;

DataBlock::DataBlock(void)
    : m_pBlockStructure(NULL)
{
}

DataBlock:: ~DataBlock(void)
{
    if (m_pBlockStructure != NULL)
    {
        delete m_pBlockStructure;
        m_pBlockStructure = NULL;
    }
}

DataBlock* DataBlock::CreateEmptyNormalBlock(const size_t block_size)
{
    return BlockCreater::CreateEmptyNormalBlock(block_size);
}

void DataBlock::CreateDataBlocks(const key_value_map_t& datas, const size_t block_size, CreateDataBlocksResult& result)
{
    return;
}

BlockStructure& DataBlock::GetBlockStructure(void)
{
    return *m_pBlockStructure;
}

const BlockStructure& DataBlock::GetBlockStructure(void) const
{
    return *m_pBlockStructure;
}
