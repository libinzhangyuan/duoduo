#ifndef DUO_BLOCK_STRUCTURE_DATA_ONLY_H_
#define DUO_BLOCK_STRUCTURE_DATA_ONLY_H_

#include <string>
#include "../typedef.h"
#include "BlockStructure.h"

namespace DuoDuo
{
    class BlockStructure_DataOnly : public BlockStructure
    {
    public:
        BlockStructure_DataOnly(block_t& block) :
            BlockStructure(block, BlockStructure::eBlockType_DataOnly),
            m_StructCalc(block.size()) {}
        virtual BlockStructure* Clone(void);

        virtual bool IsEnoughForData(const std::string& key, const std::string& value) const;
        virtual void AddData(const std::string& key, const std::string& value);
        virtual void PackBlock(void);
        virtual void LoadFromBlock(void);
        virtual size_t GetExtraBlockCount(void) const;
        //virtual std::map<std::string /*key*/, pos_in_block_t> IndexFromBlock(void) const;

    public:
        const std::string& GetValueStoredInBlock(void) const {return m_ValueStoredInBlock;}


    public:
        class StructCalc
        {
            public:
                StructCalc(size_t block_size) : m_BlockSize(block_size) {}

                size_t HeadSize(void) const;
                size_t MaxValueLen(void) const;
                size_t BlockNeedCount(const size_t data_size_need_store_in_dataonly) const;

            private:
                size_t m_BlockSize;
        };

    protected:
        StructCalc m_StructCalc;
        std::string m_ValueStoredInBlock;
    };
}

#endif
