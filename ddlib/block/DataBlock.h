#ifndef DUO_DATA_BLOCK_H_
#define DUO_DATA_BLOCK_H_

#include <string>
#include <vector>
#include "../typedef.h"

#include "BlockStructure.h"


namespace DuoDuo
{
    class BlockStructure;
    struct CreateDataBlocksResult;

    class DataBlock
    {
    public:
        DataBlock(void);
        DataBlock(const DataBlock& r);
        ~DataBlock(void);

        DataBlock& operator=(const DataBlock& r);

        static DataBlock null_block;
        bool IsNullBlock(void) {return (m_Block.size() == 0);}

        bool IsEmpty(void) const;

        static DataBlock EmptyNormalBlock(const size_t block_size);

        static std::vector<DataBlock> CreateBlockWithBigData(size_t blockSize, const std::string& key, const std::string& value);

        // if a block is a NormalBlock : return 0
        // if BigBlock                 : return the count of the extra dataOnlyBlock
        // if DataOnlyBlock            : throw exception
        static size_t GetExtraBlockCount(const DataBlock& block);

        /* usage:
         * load one block
         * extraBlockCount = GetExtraBlockCount(block);
         * load extraBlock if extraBlockCount > 0
         * GetData(blocks) */
        static key_value_map_t GetData(const std::vector<DataBlock>& blocks);

        //static void CreateDataBlocks(const key_value_map_t& datas, const size_t block_size, CreateDataBlocksResult& result);

    public:

        BlockStructure::BlockType Type(void) const;

        bool IsEnoughForData(const std::string& key, const std::string& value) const;
        void AddData(const std::string& key, const std::string& value);
        void PackBlock(void);
        void LoadFromBlock(void);

    public:
        std::string GetDebugText(void) const;

    protected:

    private:
        friend class BlockCreater;
        DataBlock(BlockStructure::BlockType blockType, size_t blockSize);

        static BlockStructure* CreateBlockStructure(BlockStructure::BlockType blockType, block_t& block);

        BlockStructure& GetBlockStructure(void);
        const BlockStructure& GetBlockStructure(void) const;
        BlockStructure* GetBlockStructurePtr(void);
        const BlockStructure* GetBlockStructurePtr(void) const;

        static std::pair<std::string /*key*/, std::string /*value*/> GetBigData(const std::vector<DataBlock>& blocks);

    private:
        block_t m_Block;
        BlockStructure* m_pBlockStructure; // strategy
    };

    //struct CreateDataBlocksResult
    //{
    //    std::vector<DataBlock> filled_blocks;  // blocks that is filled up by data.
    //    key_value_map_t left_datas; // the rest of small data that can not fill up one normalBlock.
    //    DataBlock block_of_left_datas; // the block that is not full. the data is comming from left_datas.
    //};
}

#endif
