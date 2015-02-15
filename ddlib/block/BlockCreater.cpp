#include <assert.h>

#include <check_function.h>

#include "../Util.h"
#include "../Config.h"
#include "../def.h"

#include "BlockCreater.h"
#include "BlockStructure.h"
#include "BlockStructure_Normal.h"
#include "BlockStructure_Big.h"
#include "BlockStructure_DataOnly.h"
#include "DataBlock.h"

using namespace DuoDuo;

DataBlock BlockCreater::EmptyNormalBlock(const size_t block_size)
{
    return DataBlock(BlockStructure::eBlockType_Normal, block_size);
}

DataBlock BlockCreater::EmptyBigBlock(const size_t block_size)
{
    return DataBlock(BlockStructure::eBlockType_Big, block_size);
}

DataBlock BlockCreater::EmptyDataOnlyBlock(const size_t block_size)
{
    return DataBlock(BlockStructure::eBlockType_DataOnly, block_size);
}


std::vector<DataBlock> BlockCreater::CreateBlockWithBigData(size_t blockSize, const std::string& key, const std::string& value)
{
    assert_check(BlockStructure_Normal::StructCalc(blockSize).IsNormalData(key, value) == false, "BlockCreater::CreateBlockWithBigData");
    exception_assert(key.size() <= BlockStructure_Big::StructCalc(blockSize).MaxKeyLen(), "BlockCreater::CreateBlockWithBigData");

    std::vector<DataBlock> blocks;

    // BigBlock
    DataBlock big(BlockStructure::eBlockType_Big, blockSize);
    const size_t bigBlockMaxValueLen = BlockStructure_Big::StructCalc(blockSize).MaxValueLen(key.size());
    big.AddData(key, value);
    blocks.push_back(big);

    // DataOnlyBlock
    if (value.size() > bigBlockMaxValueLen)
    {
        std::string leftValue = value.substr(bigBlockMaxValueLen, std::string::npos);
        while (leftValue.size() > 0)
        {
            DataBlock dataOnly(BlockStructure::eBlockType_DataOnly, blockSize);
            const size_t dataOnlyBlockMaxValueLen = BlockStructure_DataOnly::StructCalc(blockSize).MaxValueLen();
            dataOnly.AddData(key, value);
            blocks.push_back(dataOnly);

            const size_t valueSizeInThisBlock = std::min(dataOnlyBlockMaxValueLen, leftValue.size());
            leftValue = leftValue.substr(valueSizeInThisBlock, std::string::npos);
        }
    }

    return blocks;
}
