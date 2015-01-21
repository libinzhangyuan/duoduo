#ifndef DUO_BLOCK_STRUCTURE_H_
#define DUO_BLOCK_STRUCTURE_H_

#include <string>
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

        BlockStructure(BlockStructure::BlockType blockType) : m_BlockType(blockType) {}
        virtual ~BlockStructure(void) {}

        static BlockStructure* SelectStructure(const std::string& key, const std::string& value);
        static BlockStructure* SelectStructure(BlockType blockType);
        static BlockStructure* SelectStructure(const std::string& block); // block is truly data in a file block.

        BlockType Type(void) {return m_BlockType;}

        //virtual bool IsEnoughForData(const std::string& block, const std::string& key, const std::string& value) const = 0;
        //virtual void AppendData(std::string& block, const std::string& key, const std::string& value) = 0;

        //struct LoadDataRst
        //{
        //    bool bValueNotEnd; // indicate the value of this key is not end. (the rest is in DataBlock_DataOnly).
        //    std::map<std::string /*key*/, std::string /*value*/> key_value_data;
        //};
        //virtual std::map<std::string /*key*/, std::string /*value*/> LoadData(std::string& block) = NULL;
    private:
        BlockType m_BlockType;
    };
}

#endif
