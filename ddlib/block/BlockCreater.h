#ifndef DUO_BLOCK_CREATER_H_
#define DUO_BLOCK_CREATER_H_

#include <string>
#include "../typedef.h"

namespace DuoDuo
{
    class DataBlock;

    class BlockCreater
    {
    public:
        BlockCreater(void) {}
        virtual ~BlockCreater(void) {}

        static DataBlock* CreateEmptyNormalBlock(const size_t block_size);
        static DataBlock* CreateEmptyDataOnlyBlock(const size_t block_size);

    protected:

    private:

    private:
    };
}

#endif
