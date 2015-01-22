#ifndef DUO_DATA_BLOCK_H_
#define DUO_DATA_BLOCK_H_

#include <string>
#include <vector>
#include "../typedef.h"

#include "BlockStructure.h"

namespace DuoDuo
{
    class DataBlock
    {
    public:
        ~DataBlock(void);

        static std::vector<DataBlock> CreateDataBlocks(const std::string& key, const std::string& value);
        static DataBlock CreateEmptyNormalBlock(void);

        // checking the capacitance of the rest of DataBlock.
        bool IsEnoughForData(const std::string& key, const std::string& value) const;

        void AppendData(const std::string& key, const std::string& value);
        bool IsBigData(const std::string& key, const std::string& value) const;

    protected:

    private:
        DataBlock(void);

        BlockStructure& GetBlockStructure(void);
        const BlockStructure& GetBlockStructure(void) const;

    private:
        std::string m_Block;
        BlockStructure* m_pBlockStructure;
    };
}

#endif
