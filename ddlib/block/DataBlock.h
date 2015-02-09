#ifndef DUO_DATA_BLOCK_H_
#define DUO_DATA_BLOCK_H_

#include <string>
#include <vector>
#include "../typedef.h"


namespace DuoDuo
{
    class BlockStructure;
    struct CreateDataBlocksResult;

    class DataBlock
    {
    public:
        ~DataBlock(void);

        static DataBlock* CreateEmptyNormalBlock(const size_t block_size);
        static void CreateDataBlocks(const key_value_map_t& datas, const size_t block_size, CreateDataBlocksResult& result);

        // checking the capacitance of the rest of DataBlock.
        //bool IsEnoughForData(const std::string& key, const std::string& value) const;

        //void AppendData(const std::string& key, const std::string& value);
        //bool IsBigData(const std::string& key, const std::string& value) const;

    protected:

    private:
        DataBlock(void);

        BlockStructure& GetBlockStructure(void);
        const BlockStructure& GetBlockStructure(void) const;

    private:
        block_t m_Block;
        BlockStructure* m_pBlockStructure; // strategy
    };

    struct CreateDataBlocksResult
    {
        std::vector<DataBlock> filled_blocks;  // blocks that is filled up by data.
        key_value_map_t left_datas; // the rest of small data that can not fill up one normalBlock.
        DataBlock block_of_left_datas; // the block that is not full. the data is comming from left_datas.
    };
}

#endif
