
#include "DataBlock.h"
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

std::vector<DataBlock> DataBlock::CreateDataBlocks(const std::string& key, const std::string& value)
{
    return std::vector<DataBlock>();
}

BlockStructure& DataBlock::GetBlockStructure(void)
{
    return *m_pBlockStructure;
}

const BlockStructure& DataBlock::GetBlockStructure(void) const
{
    return *m_pBlockStructure;
}
