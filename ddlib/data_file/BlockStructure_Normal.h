#ifndef DUO_BLOCK_STRUCTURE_NORMAL_H_
#define DUO_BLOCK_STRUCTURE_NORMAL_H_

#include <string>
#include <binary_stream/binary_buf.hpp>
#include "../typedef.h"
#include "BlockStructure.h"

namespace CppUnit {
    struct Asserter;
};

namespace DuoDuo
{
    class BlockStructure_Normal : public BlockStructure
    {
    public:

        BlockStructure_Normal(block_t& block);
        ~BlockStructure_Normal(void) {}

        virtual bool IsEnoughForData(const std::string& key, const std::string& value) const;
        virtual void AddData(const std::string& key, const std::string& value);
        //virtual void LoadFromBlock(void);

    private:
        typedef pos_in_block_t data_pos_t;
        static const data_pos_t DATA_POS_OF_DELETE_IDENTITY; //data pos = 0 indicate this 'key/value' is deleted.

        size_t KeySectionSize(void) const;
        size_t DataSectionSize(void) const;

        size_t KeySection_BodySize(void) const;
        //size_t KeySection_BodyUsedCount(void) const;
        size_t KeySection_BodyUsedCount_Except(const std::string& key) const;

        size_t DataSection_BodySize(void) const;
        //size_t DataSection_BodyUsedCount(void) const;
        size_t DataSection_BodyUsedCount_Except(const std::string& key) const;


        bool IsEnoughForKeySection(const std::string& key) const;
        bool IsEnoughForDataSection(const std::string& key, const std::string& value) const;

        void AddDataToKeyValueMap(const std::string& key, const std::string& value);

        static size_t GetKeyNeedLen(const std::string& key);
        static size_t GetValueNeedLen(const std::string& value);

        static Essential::_binary_buf MakeKeyBuffer(const std::string& key, const data_pos_t& data_pos);
        static Essential::_binary_buf MakeDeletedKeyBuffer(const std::string& key);
    private:
        size_t m_KeySectionSize;
        size_t m_DataSectionSize;
        friend class BlockStructure_NormalTest;
        key_value_map_t m_KeyValues;
    };
}

#endif
