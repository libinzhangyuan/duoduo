#include "BlockStructure_DataOnly.h"
#include "../def.h"

using namespace DuoDuo;


size_t BlockStructure_DataOnly::ValueSize_CanStoreToBlock(const size_t& key_len, const size_t& block_size)
{
    return block_size - DD_BLOCK_HEAD_SIZE;
}
