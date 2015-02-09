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

using namespace DuoDuo;

DataBlock* BlockCreater::CreateEmptyNormalBlock(const size_t block_size)
{
    return NULL;
}
