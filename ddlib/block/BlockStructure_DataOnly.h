#ifndef DUO_BLOCK_STRUCTURE_DATA_ONLY_H_
#define DUO_BLOCK_STRUCTURE_DATA_ONLY_H_

#include <string>
#include "../typedef.h"
#include "BlockStructure.h"

namespace DuoDuo
{
    class BlockStructure_DataOnly : public BlockStructure
    {
    public:
        BlockStructure_DataOnly(block_t& block) :
            BlockStructure(block, BlockStructure::eBlockType_DataOnly) {}
    };
}

#endif
