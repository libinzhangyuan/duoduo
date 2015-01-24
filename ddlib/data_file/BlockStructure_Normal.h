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

        virtual bool IsEnoughForData(const block_t& block, const std::string& key, const std::string& value) const;
        virtual void AppendData(block_t& block, const std::string& key, const std::string& value);

    private:
        typedef pos_in_block_t data_pos_t;
        static const data_pos_t DATA_POS_OF_DELETE_IDENTITY; //data pos = 0 indicate this 'key/value' is deleted.

        size_t KeySectionSize(void) const;
        size_t DataSectionSize(void) const;

        static size_t GetKeyNeedLen(const std::string& key);
        static size_t GetValueNeedLen(const std::string& value);

        static Essential::_binary_buf MakeKeyBuffer(const std::string& key, const data_pos_t& data_pos);
        static Essential::_binary_buf MakeDeletedKeyBuffer(const std::string& key);
    private:
        size_t m_KeySectionSize;
        size_t m_DataSectionSize;
        friend class BlockStructure_NormalTest;
    };
}

#endif
