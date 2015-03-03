#include "BlockStructure_Big.h"
#include "../def.h"

#include <check_function.h>
#include <binary_stream/binary_istream.h>
#include <binary_stream/binary_ostream.h>
#include "BlockStructure_DataOnly.h"

using namespace DuoDuo;

namespace DuoDuo
{
    namespace BigBlock
    {
        typedef uint16_t key_len_t;
        typedef uint32_t data_len_t;
        typedef char key_check_num_t;

        const char KEY_CHECK_NUM = char(0xF1);
    }

}

namespace DuoDuo
{
    size_t BlockStructure_Big::StructCalc::HeadSize(void) const
    {
        return DD_BLOCK_HEAD_SIZE;
    }

    size_t BlockStructure_Big::StructCalc::MaxKeyLen(void) const
    {
        return m_BlockSize - HeadSize() - sizeof(BigBlock::key_len_t)
            - sizeof(BigBlock::key_check_num_t) - sizeof(BigBlock::data_len_t) - sizeof(extra_block_count_t);
    }

    size_t BlockStructure_Big::StructCalc::MaxValueLen(size_t key_len) const
    {
        return m_BlockSize - HeadSize() - sizeof(BigBlock::key_len_t) - key_len
            - sizeof(BigBlock::key_check_num_t) - sizeof(BigBlock::data_len_t) - sizeof(extra_block_count_t);
    }
}

BlockStructure* BlockStructure_Big::Clone(void)
{
    BlockStructure_Big* pClone = new BlockStructure_Big(m_Block);
    pClone->m_BlockType = m_BlockType;
    pClone->m_BlockSize = m_BlockSize;
    pClone->m_Key = m_Key;
    pClone->m_ValueTotalLen = m_ValueTotalLen;
    pClone->m_StructCalc = m_StructCalc;
    pClone->m_ValueStoredInBlock = m_ValueStoredInBlock;
    return pClone;
}

bool BlockStructure_Big::IsEnoughForData(const std::string& key, const std::string& value) const
{
    return value.size() <= m_StructCalc.MaxValueLen(key.size());
}

void BlockStructure_Big::AddData(const std::string& key, const std::string& value)
{
    assert_check(key.size() > 0, "BlockStructure_Big::AddData");

    m_Key = key;
    m_ValueTotalLen = value.size();
    const size_t max_value_size_in_block = m_StructCalc.MaxValueLen(key.size());
    if (value.size() > max_value_size_in_block)
        m_ValueStoredInBlock = value.substr(0, max_value_size_in_block);
    else
        m_ValueStoredInBlock = value;
}

void BlockStructure_Big::PackBlock(void)
{
    assert_check(m_Key.size() > 0, "BlockStructure_Big::AddData");
    assert_check(IsEnoughForData(m_Key, m_ValueStoredInBlock) == true, "BlockStructure_Big::AddData");

    CleanBody();

    Essential::_binary_ostream<Essential::_binary_buf> bfstream(m_Block);

    // head
    InitHead();
    bfstream.SetWritePos(HeadSize());

    // key len
    bfstream.Pack<BigBlock::key_len_t>(m_Key.size());

    // key
    bfstream.Pack(m_Key.c_str(), m_Key.size());

    // checknum
    bfstream.Pack<BigBlock::key_check_num_t>(BigBlock::KEY_CHECK_NUM);

    // value total len
    bfstream.Pack<BigBlock::data_len_t>(m_ValueTotalLen);

    // extra block count
    extra_block_count_t extraBlockCount = CalcExtraBlockCount();
    bfstream.Pack<extra_block_count_t>(extraBlockCount);

    // data
    bfstream.Pack(m_ValueStoredInBlock.c_str(), m_ValueStoredInBlock.size());
}

BlockStructure_Big::extra_block_count_t BlockStructure_Big::CalcExtraBlockCount(void) const
{
    const size_t left_value_size = m_ValueTotalLen - m_ValueStoredInBlock.size();
    return BlockStructure_DataOnly::StructCalc(m_Block.size()).BlockNeedCount(left_value_size);
}

void BlockStructure_Big::LoadFromBlock(void)
{
    // check head
    assert_check(GetBlockTypeFromBlock() == BlockStructure::eBlockType_Big, "BlockStructure_Big::LoadFromBlock : check head");

    Essential::_binary_istream<Essential::_binary_buf> bstream(GetBlock());

    // key_section_stream:  move head
    bstream.MoveReadPos(HeadSize());

    // key len
    const BigBlock::key_len_t key_len = bstream.Unpack<BigBlock::key_len_t>();
    exception_assert(key_len > 0, "BigBlock Load: keyLen must larger than zero!");

    // key
    exception_assert(key_len < bstream.LeftBytes(), "BigBlock Load: key out of range!");
    m_Key = bstream.Unpack(key_len);

    // check num
    exception_assert(sizeof(BigBlock::key_check_num_t) < bstream.LeftBytes(), "BigBlock Load: checkNum out of range!");
    const BigBlock::key_check_num_t checkNum = bstream.Unpack<BigBlock::key_check_num_t>();
    exception_assert(checkNum == BigBlock::KEY_CHECK_NUM, "BigBlock Load: checkNum wrong code!");

    // value total len
    exception_assert(sizeof(BigBlock::data_len_t) < bstream.LeftBytes(), "BigBlock Load: dataLen out of range!");
    m_ValueTotalLen = bstream.Unpack<BigBlock::data_len_t>();
    exception_assert(m_ValueTotalLen >= 0, "BigBlock Load: data_len must larger than zero");

    // extra block count
    exception_assert(sizeof(extra_block_count_t) <= bstream.LeftBytes(), "BigBlock Load: extra_block_count out of range!");
    /*const extra_block_count_t extra_block_count = */ bstream.Unpack<extra_block_count_t>();

    // value
    size_t value_size_in_this_block = (bstream.LeftBytes() < m_ValueTotalLen) ? bstream.LeftBytes() : m_ValueTotalLen;
    m_ValueStoredInBlock = bstream.Unpack(value_size_in_this_block);
}

size_t BlockStructure_Big::GetExtraBlockCount(void) const
{
    return CalcExtraBlockCount();
}
