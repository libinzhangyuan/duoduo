#include <assert.h>
#include "../Util.h"

#include "BlockStructure.h"
#include "BlockStructure_Normal.h"
#include "BlockStructure_Big.h"
#include "BlockStructure_DataOnly.h"

using namespace DuoDuo;

static BlockStructure_Normal static_BlockStructure_Normal;
static BlockStructure_Big static_BlockStructure_Big;
static BlockStructure_DataOnly static_BlockStructure_DataOnly;

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
