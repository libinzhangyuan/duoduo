#ifndef DUO_BLOCK_STRUCTURE_NORMAL_H_
#define DUO_BLOCK_STRUCTURE_NORMAL_H_

#include <string>
#include <binary_stream/binary_buf.hpp>
#include <binary_stream/binary_istream.h>
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

        virtual void LoadFromBlock(void);
        virtual std::map<std::string /*key*/, pos_in_block_t> IndexFromBlock(void) const;

        virtual bool IsEnoughForData(const std::string& key, const std::string& value) const;
        virtual void AddData(const std::string& key, const std::string& value);
        virtual void PackBlock(void);

    private:
        typedef pos_in_block_t data_pos_t;

        static const data_pos_t DATA_POS_OF_DELETE_IDENTITY; //data pos = 0 indicate this 'key/value' is deleted.

        //size_t KeySection_BodyUsedCount(void) const;
        size_t KeySection_BodyUsedCount_Except(const std::string& key) const;

        //size_t DataSection_BodyUsedCount(void) const;
        size_t DataSection_BodyUsedCount_Except(const std::string& key) const;


        bool IsEnoughForKeySection(const std::string& key) const;
        bool IsEnoughForDataSection(const std::string& key, const std::string& value) const;

        void AddDataToKeyValueMap(const std::string& key, const std::string& value);


        struct KeyDataBodyResult
        {
            Essential::_binary_buf key_section_body;
            Essential::_binary_buf data_section_body;
        };
        KeyDataBodyResult MakeKeySectionBodyAndDataSectionBody(const key_value_map_t& key_values) const;

        static size_t GetKeyNeedLen(const std::string& key);
        static size_t GetValueNeedLen(const std::string& value);

        static Essential::_binary_buf MakeKeyBuffer(const std::string& key, const data_pos_t& data_pos);
        static Essential::_binary_buf MakeDeletedKeyBuffer(const std::string& key);
        static Essential::_binary_buf MakeValueBuffer(const std::string& value);

        static std::string GetKey(Essential::_binary_istream<Essential::_binary_buf>& key_section_stream, data_pos_t& data_pos);
        static std::string GetValue(Essential::_binary_istream<Essential::_binary_buf>& data_section_stream);
        std::string GetValue(const data_pos_t& data_pos) const;

    private:
        class StructCalc
        {
            public:
                StructCalc(size_t block_size) : m_BlockSize(block_size) {}

                size_t HeadSize(void) const;
                size_t KeySectionSize(void) const;
                size_t KeySection_BodySize(void) const;

                size_t DataSectionSize(void) const;
                size_t DataSectionHeadSize(void) const;
                size_t DataSection_BodySize(void) const;
                BlockStructure::pos_in_block_t DataSection_BodyStartPos(void) const;

            private:
                size_t m_BlockSize;
        };



    private:
        key_value_map_t m_KeyValues;
        StructCalc m_StructCalc;
    };
}

#endif
