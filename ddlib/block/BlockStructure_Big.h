#ifndef DUO_BLOCK_STRUCTURE_BIG_H_
#define DUO_BLOCK_STRUCTURE_BIG_H_

#include <string>
#include "../typedef.h"
#include "BlockStructure.h"

namespace DuoDuo
{
    class BlockStructure_Big : public BlockStructure
    {
    public:
        typedef uint16_t extra_block_count_t; // The count of extra DataOnlyBlock.
                // - Pack extra data to DataOnlyBlock after BigBlock when data is larger than BigBlock.

    public:
        BlockStructure_Big(block_t& block) : BlockStructure(block, BlockStructure::eBlockType_Big)
                                           , m_ValueTotalLen(-1), m_StructCalc(block.size()) {}
        virtual BlockStructure* Clone(block_t& blockNewStructureBindingTo);

        virtual bool IsEnoughForData(const std::string& key, const std::string& value) const;

        /* usage for big data
         * std::string key, value;
         * store_len = bigStructure::StructCalc.MaxValueLen(key.size());
         * big.AddData(key, value);
         * big.PackBlock();
         * dataOnly.AddData(key, value[store_len .. store_len + dataOnlyMaxDataLen]);
         * dataOnly.PackBlock();
         * ...
        */
        virtual void AddData(const std::string& key, const std::string& value);

        virtual void PackBlock(void);

        virtual void LoadFromBlock(void);

        virtual size_t GetExtraBlockCount(void) const;

        //virtual std::map<std::string /*key*/, pos_in_block_t> IndexFromBlock(void) const;

    public:
        const std::string& GetKey(void) const {return m_Key;}
        const std::string& GetValueStoredInBlock(void) const {return m_ValueStoredInBlock;}
        size_t GetValueTotalLen(void) const {return m_ValueTotalLen;}

    public:
        class StructCalc
        {
            public:
                StructCalc(size_t block_size) : m_BlockSize(block_size) {}

                size_t HeadSize(void) const;
                size_t MaxKeyLen(void) const;
                size_t MaxValueLen(size_t key_len) const;

            private:
                size_t m_BlockSize;
        };

    public:
        virtual std::string GetDebugText(void) const;

    private:
        extra_block_count_t CalcExtraBlockCount(void) const;

    private:
        std::string m_Key;
        std::string m_ValueStoredInBlock; // the value that stored in this block. (Some value may be stored in sequential DataOnlyBlock if value too large);
        size_t m_ValueTotalLen;
        StructCalc m_StructCalc;
    };
}

#endif
