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
            None = 0,
            Normal = 1,
            Big = 2,
            DataOnly = 3
        };

        BlockStructure(void) {}
        virtual ~BlockStructure(void) {}

        static BlockStructure* SelectStructure(const std::string& key, const std::string& value);
        static BlockStructure* SelectStructure(BlockType blockType);
        static BlockStructure* SelectStructure(const std::string& block); // block is truly data in a file block.

        virtual bool IsEnoughForData(const std::string& block, const std::string& key, const std::string& value) const = 0;
        virtual void AppendData(std::string& block, const std::string& key, const std::string& value) = 0;

        //struct LoadDataRst
        //{
        //    bool bValueNotEnd; // indicate the value of this key is not end. (the rest is in DataBlock_DataOnly).
        //    std::map<std::string /*key*/, std::string /*value*/> key_value_data;
        //};
        //virtual std::map<std::string /*key*/, std::string /*value*/> LoadData(std::string& block) = NULL;
    private:
    };
}

#endif
