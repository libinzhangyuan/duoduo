#include "BlockStructure.h"
#include "BlockStructure_Normal.h"
#include "BlockStructure_Big.h"

using namespace DuoDuo;

static BlockStructure_Normal static_BlockStructure_Normal;
//static BlockStructure_Big static_BlockStructure_Big;

BlockStructure* BlockStructure::SelectStructure(const std::string& key, const std::string& value)
{
    return &static_BlockStructure_Normal;

    // if BlockStructure_Normal enough
    //     return static_BlockStructure_Normal
    // else
    //     return static_BlockStructure_Big
}

BlockStructure* BlockStructure::SelectStructure(BlockType blockType)
{
    return &static_BlockStructure_Normal;
}

BlockStructure* BlockStructure::SelectStructure(const std::string& block)
{
    return &static_BlockStructure_Normal;
}
