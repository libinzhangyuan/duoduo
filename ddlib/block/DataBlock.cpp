
#include "BlockStructure.h"
#include "DataBlock.h"
#include "BlockCreater.h"
#include "../Util.h"
#include "BlockStructure_Normal.h"
#include "BlockStructure_Big.h"
#include "BlockStructure_DataOnly.h"

using namespace DuoDuo;

DataBlock DataBlock::null_block;
DataBlock::DataBlock(void)
    : m_pBlockStructure(NULL)
{
}

DataBlock::DataBlock(const DataBlock& r)
    : m_Block(r.m_Block)
    , m_pBlockStructure(r.m_pBlockStructure->Clone(m_Block))
{
}

DataBlock::DataBlock(BlockStructure::BlockType blockType, size_t blockSize)
    : m_pBlockStructure(NULL)
{
    m_Block.resize(blockSize);
    m_pBlockStructure = CreateBlockStructure(blockType, m_Block);
}

DataBlock& DataBlock::operator=(const DataBlock& r)
{
    if (&r == this)
        return *this;

    m_Block = r.m_Block;
    if (m_pBlockStructure != NULL)
    {
        delete m_pBlockStructure;
        m_pBlockStructure = NULL;
    }
    m_pBlockStructure = r.m_pBlockStructure->Clone(m_Block);
    return *this;
}

BlockStructure* DataBlock::CreateBlockStructure(BlockStructure::BlockType blockType, block_t& block)
{
    switch (blockType)
    {
        case BlockStructure::eBlockType_Normal: return new BlockStructure_Normal(block);
        case BlockStructure::eBlockType_Big: return new BlockStructure_Big(block);
        case BlockStructure::eBlockType_DataOnly: return new BlockStructure_DataOnly(block);
        default:
            char msg[1024] = "";
            snprintf(msg, sizeof(msg), "DataBlock::DataBlock wrong blockType: %d", blockType);
            assert_check(false, "DataBlock::DataBlock wrong blockType");
            return NULL;
    }
}

DataBlock::~DataBlock(void)
{
    if (m_pBlockStructure != NULL)
    {
        delete m_pBlockStructure;
        m_pBlockStructure = NULL;
    }
}

BlockStructure::BlockType DataBlock::Type(void) const
{
    assert_check(m_pBlockStructure != NULL, "DataBlock::Type");
    return m_pBlockStructure->Type();
}

DataBlock DataBlock::EmptyNormalBlock(const size_t block_size)
{
    return BlockCreater::EmptyNormalBlock(block_size);
}

bool DataBlock::IsEmpty(void) const
{
    return GetBlockStructure().IsEmpty();
}

std::vector<DataBlock> DataBlock::CreateBlockWithBigData(size_t blockSize, const std::string& key, const std::string& value)
{
    return BlockCreater::CreateBlockWithBigData(blockSize, key, value);
}

/*
void DataBlock::CreateDataBlocks(const key_value_map_t& datas, const size_t block_size, CreateDataBlocksResult& result)
{
    return;
}
*/

BlockStructure& DataBlock::GetBlockStructure(void)
{
    return *m_pBlockStructure;
}

const BlockStructure& DataBlock::GetBlockStructure(void) const
{
    return *m_pBlockStructure;
}

const BlockStructure* DataBlock::GetBlockStructurePtr(void) const
{
    return m_pBlockStructure;
}

BlockStructure* DataBlock::GetBlockStructurePtr(void)
{
    return m_pBlockStructure;
}

bool DataBlock::IsEnoughForData(const std::string& key, const std::string& value) const
{
    return GetBlockStructure().IsEnoughForData(key, value);
}

void DataBlock::AddData(const std::string& key, const std::string& value)
{
    GetBlockStructure().AddData(key, value);
}

void DataBlock::PackBlock(void)
{
    GetBlockStructure().PackBlock();
}

void DataBlock::LoadFromBlock(void)
{
    GetBlockStructure().LoadFromBlock();
}

// if a block is a NormalBlock : return 0
// if BigBlock                 : return the count of the extra dataOnlyBlock
// if DataOnlyBlock            : throw exception
size_t DataBlock::GetExtraBlockCount(const DataBlock& block)
{
    return block.GetBlockStructure().GetExtraBlockCount();
}

/* usage:
 * load one block
 * extraBlockCount = GetExtraBlockCount(block);
 * load extraBlock if extraBlockCount > 0
 * GetData(blocks) */
key_value_map_t DataBlock::GetData(const std::vector<DataBlock>& blocks)
{
    assert_check(blocks.size() > 0, "DataBlock::GetData");
    const DataBlock& first_block = blocks[0];
    switch (first_block.Type())
    {
        case BlockStructure::eBlockType_Normal:
            {
                return first_block.GetBlockStructure().GetDatas();
            }
        case BlockStructure::eBlockType_Big:
            {
                std::pair<std::string /*key*/, std::string /*value*/> data = GetBigData(blocks);
                key_value_map_t ret;
                ret[data.first] = data.second;
                return ret;
            }
        case BlockStructure::eBlockType_DataOnly:
            {
                assert_check(false, "DataBlock::GetData : first block is DataOnlyBlock");
                return key_value_map_t();
            }
        default:
            {
                assert_check(false, "DataBlock::GetData : unknown type of first block");
                return key_value_map_t();
            }
    }
}

key_value_map_t DataBlock::GetData(const DataBlock& block)
{
    const DataBlock& first_block = block;
    assert_check(block.Type() == BlockStructure::eBlockType_Normal, "DataBlock::GetData from one block");
    return first_block.GetBlockStructure().GetDatas();
}

std::pair<std::string /*key*/, std::string /*value*/> DataBlock::GetBigData(const std::vector<DataBlock>& blocks)
{
    const DataBlock& big_block = blocks[0];
    assert_check(big_block.Type() == BlockStructure::eBlockType_Big, "DataBlock::GetBigData");

    const std::string& key = big_block.GetBlockStructure().GetKey();

    std::string value = big_block.GetBlockStructure().GetValueStoredInBlock();
    const size_t extraBlockCount = big_block.GetBlockStructure().GetExtraBlockCount();
    exception_assert(extraBlockCount == (blocks.size() - 1), "DataBlock::GetBigData");
    for (size_t i = 1; i <= extraBlockCount; ++i)
    {
        const DataBlock& dataOnlyBlock = blocks[i];
        value.append(dataOnlyBlock.GetBlockStructure().GetValueStoredInBlock());
    }

    return std::make_pair(key, value);
}

std::string DataBlock::GetDebugText(void) const
{
    return GetBlockStructure().GetDebugText();
}
