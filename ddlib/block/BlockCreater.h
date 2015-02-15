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

        static DataBlock EmptyNormalBlock(const size_t block_size);
        static DataBlock EmptyBigBlock(const size_t block_size);
        static DataBlock EmptyDataOnlyBlock(const size_t block_size);

        static std::vector<DataBlock> CreateBlockWithBigData(size_t blockSize, const std::string& key, const std::string& value);

    protected:

    private:

    private:
    };
}

#endif
