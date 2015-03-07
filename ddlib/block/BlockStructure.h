#ifndef DUO_BLOCK_STRUCTURE_H_
#define DUO_BLOCK_STRUCTURE_H_

#include <string>
#include <stdint.h>
#include <binary_stream/binary_buf.hpp>
#include "../typedef.h"

namespace DuoDuo
{
    class BlockStructure
    {
    public:
        enum BlockType
        {
            eBlockType_None = 0,
            eBlockType_Normal = 1,
            eBlockType_Big = 2,
            eBlockType_DataOnly = 3
        };

        BlockStructure(block_t& block, BlockStructure::BlockType blockType) :
            m_Block(block), m_BlockType(blockType), m_BlockSize(block.size()) {}
        virtual ~BlockStructure(void) {}

        virtual BlockStructure* Clone(block_t& blockNewStructureBindingTo) = 0;

        //static BlockStructure* SelectStructure(const std::string& key, const std::string& value);
        static BlockStructure* Create(block_t& block, BlockType blockType);
        //static BlockStructure* SelectStructure(const block_t& block); // block is truly data in a file block.

        BlockType Type(void) {return m_BlockType;}

        virtual void InitBlock(void) const;
        virtual bool IsEnoughForData(const std::string& key, const std::string& value) const = 0;
        virtual void AddData(const std::string& key, const std::string& value) = 0;
        virtual void PackBlock(void) = 0;
        virtual void LoadFromBlock(void) = 0;
        virtual size_t GetExtraBlockCount(void) const = 0;
        //virtual std::map<std::string /*key*/, pos_in_block_t> IndexFromBlock(void) const = 0;

        virtual const key_value_map_t& GetDatas(void) const;
        virtual const std::string& GetKey(void) const;
        virtual const std::string& GetValueStoredInBlock(void) const;

        //struct LoadDataRst
        //{
        //    bool bValueNotEnd; // indicate the value of this key is not end. (the rest is in DataBlock_DataOnly).
        //    std::map<std::string /*key*/, std::string /*value*/> key_value_data;
        //};
        //virtual std::map<std::string /*key*/, std::string /*value*/> LoadData(std::string& block) = NULL;

        virtual std::string GetDebugText(void) const;

    protected:
        size_t HeadSize(void) const;
        void CleanBody(void);
        BlockType GetBlockTypeFromBlock(void) const;
        block_t& GetBlock(void) {return m_Block;}
        const block_t& GetBlock(void) const {return m_Block;}
        BlockType GetBlockType(void) const {return m_BlockType;}
        const size_t BlockSize(void) const {return m_Block.size();}
        void InitHead(void) const;

    protected:
        block_t& m_Block; // --known

        BlockType m_BlockType;
        size_t m_BlockSize;
    };
}

#endif
