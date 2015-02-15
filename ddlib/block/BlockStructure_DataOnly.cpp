#include "BlockStructure_DataOnly.h"

#include <check_function.h>
#include <binary_stream/binary_istream.h>
#include <binary_stream/binary_ostream.h>

#include "../def.h"

using namespace DuoDuo;

namespace DuoDuo
{
    namespace DataOnlyBlock
    {
        typedef uint16_t data_len_t;
    }

}


namespace DuoDuo
{
    size_t BlockStructure_DataOnly::StructCalc::HeadSize(void) const
    {
        return DD_BLOCK_HEAD_SIZE;
    }

    size_t BlockStructure_DataOnly::StructCalc::MaxValueLen(void) const
    {
        return m_BlockSize - DD_BLOCK_HEAD_SIZE - sizeof(DataOnlyBlock::data_len_t);
    }

    size_t BlockStructure_DataOnly::StructCalc::BlockNeedCount(const size_t data_size_need_store_in_dataonly) const
    {
        size_t count = 0;
        size_t max_value_len = MaxValueLen();
        long long data_size = data_size_need_store_in_dataonly;
        while (data_size > 0)
        {
            count++;
            data_size -= max_value_len;
        }
        return count;
    }
}

BlockStructure* BlockStructure_DataOnly::Clone(void)
{
    BlockStructure_DataOnly* pClone = new BlockStructure_DataOnly(m_Block);
    pClone->m_BlockType = m_BlockType;
    pClone->m_BlockSize = m_BlockSize;
    pClone->m_StructCalc = m_StructCalc;
    pClone->m_ValueStoredInBlock = m_ValueStoredInBlock;
    return pClone;
}

bool BlockStructure_DataOnly::IsEnoughForData(const std::string& key, const std::string& value) const
{
    assert_check(false, "BlockStructure_DataOnly::IsEnoughForData:  should not call this function!");
    return false;
}

void BlockStructure_DataOnly::AddData(const std::string& /*key*/, const std::string& value)
{
    const size_t max_value_size_in_block = m_StructCalc.MaxValueLen();
    if (value.size() > max_value_size_in_block)
        m_ValueStoredInBlock = value.substr(0, max_value_size_in_block);
    else
        m_ValueStoredInBlock = value;
}

void BlockStructure_DataOnly::PackBlock(void)
{
    CleanBody();

    Essential::_binary_ostream<Essential::_binary_buf> bfstream(m_Block);

    // head
    InitHead();
    bfstream.SetWritePos(HeadSize());

    // data len
    bfstream.Pack<DataOnlyBlock::data_len_t>(m_ValueStoredInBlock.size());

    // data
    bfstream.Pack(m_ValueStoredInBlock.c_str(), m_ValueStoredInBlock.size());
}

void BlockStructure_DataOnly::LoadFromBlock(void)
{
    // check head
    assert_check(GetBlockTypeFromBlock() == BlockStructure::eBlockType_DataOnly, "BlockStructure_Big::LoadFromBlock : check head");

    Essential::_binary_istream<Essential::_binary_buf> bstream(GetBlock());

    // key_section_stream:  move head
    bstream.MoveReadPos(HeadSize());

    // data len
    exception_assert(sizeof(DataOnlyBlock::data_len_t) < bstream.LeftBytes(), "DataOnlyBlock Load: dataLen out of range!");
    const DataOnlyBlock::data_len_t data_len = bstream.Unpack<DataOnlyBlock::data_len_t>();
    exception_assert(data_len >= 0, "DataOnlyBlock Load: data_len must larger than zero");
    exception_assert(data_len <= m_StructCalc.MaxValueLen(), "DataOnlyBlock Load: data_len must less than or equal to block capacity!");

    // data
    m_ValueStoredInBlock = bstream.Unpack(data_len);
}

size_t BlockStructure_DataOnly::GetExtraBlockCount(void) const
{
    assert_check(false, "BlockStructure_DataOnly::GetExtraBlockCount should not call this function!");
    return 0;
}
