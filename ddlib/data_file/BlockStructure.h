#ifndef DUO_BLOCK_STRUCTURE_H_
#define DUO_BLOCK_STRUCTURE_H_

#include <string>
#include <binary_stream/binary_buf.hpp>
#include "../typedef.h"

namespace DuoDuo
{
    class BlockStructure
    {
    public:
        typedef uint16_t pos_in_block_t;  // position in one block
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

        //static BlockStructure* SelectStructure(const std::string& key, const std::string& value);
        static BlockStructure* Create(std::string& block, BlockType blockType);
        //static BlockStructure* SelectStructure(const std::string& block); // block is truly data in a file block.

        BlockType Type(void) {return m_BlockType;}

        virtual void InitBlock(void) const;
        //virtual bool IsEnoughForData(const block_t& block, const std::string& key, const std::string& value) const = 0;
        //virtual void AppendData(block_t& block, const std::string& key, const std::string& value) = 0;

        //struct LoadDataRst
        //{
        //    bool bValueNotEnd; // indicate the value of this key is not end. (the rest is in DataBlock_DataOnly).
        //    std::map<std::string /*key*/, std::string /*value*/> key_value_data;
        //};
        //virtual std::map<std::string /*key*/, std::string /*value*/> LoadData(std::string& block) = NULL;

    protected:
        size_t HeadSize(void) const;
    private:
        void InitHead(void) const;

    private:
        std::string& m_Block;
        BlockType m_BlockType;
        size_t m_BlockSize;
    };
}

#endif
