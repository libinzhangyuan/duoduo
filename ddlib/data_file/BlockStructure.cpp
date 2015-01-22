#include <assert.h>

#include <check_function.h>

#include "../Util.h"
#include "../Config.h"

#include "BlockStructure.h"
#include "BlockStructure_Normal.h"
#include "BlockStructure_Big.h"
#include "BlockStructure_DataOnly.h"

using namespace DuoDuo;

/*
static BlockStructure_Normal static_BlockStructure_Normal(Config::Ins().ssd_block_size());
static BlockStructure_Big static_BlockStructure_Big(Config::Ins().ssd_block_size());
static BlockStructure_DataOnly static_BlockStructure_DataOnly(Config::Ins().ssd_block_size());

BlockStructure* BlockStructure::SelectStructure(const std::string& key, const std::string& value)
{
    return &static_BlockStructure_Normal;

    // if BlockStructure_Normal enough
    //     return static_BlockStructure_Normal
    // else
    //     return static_BlockStructure_Big
}

BlockStructure* BlockStructure::SelectStructure(BlockType blockType, size_t block_size)
{
    switch (blockType)
    {
        case BlockStructure::eBlockType_Normal: return &static_BlockStructure_Normal;
        case BlockStructure::eBlockType_Big: return &static_BlockStructure_Big;
        case BlockStructure::eBlockType_DataOnly: return &static_BlockStructure_DataOnly;
        default: assert_check(false, "BlockStructure::SelectStructure BlockType default");
    }
    return NULL;
}

BlockStructure* BlockStructure::SelectStructure(const std::string& block)
{
    return &static_BlockStructure_Normal;
}
*/

BlockStructure* BlockStructure::Create(std::string& block, BlockType blockType)
{
    switch (blockType)
    {
        case BlockStructure::eBlockType_Normal: return new BlockStructure_Normal(block);
        case BlockStructure::eBlockType_Big: return new BlockStructure_Big(block);
        case BlockStructure::eBlockType_DataOnly: return new BlockStructure_DataOnly(block);
        default: assert_check(false, "BlockStructure::Create BlockType default");
    }
    return NULL;
}

void BlockStructure::InitBlock(void) const
{
    InitHead();
}

void BlockStructure::InitHead(void) const
{
    m_Block[2] = m_BlockType;
}

size_t BlockStructure:: HeadSize(void) const
{
    return 4;
}
