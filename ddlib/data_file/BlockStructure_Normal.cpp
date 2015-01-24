#include "../Config.h"

#include <check_function.h>
#include <binary_stream/binary_istream.h>
#include <binary_stream/binary_ostream.h>

#include "BlockStructure_Normal.h"

using namespace DuoDuo;

namespace DuoDuo
{
    namespace NormalBlock
    {
        typedef uint16_t key_len_t;
        typedef uint16_t data_len_t;
        typedef char key_check_num_t;
        typedef char data_check_num_t;

        const char KEY_CHECK_NUM = char(0xF1);
        const char DATA_CHECK_NUM = char(0xF2);
    }

}

//Setting data pos to 0 to indicating this 'key/value' is deleted.
const BlockStructure_Normal::data_pos_t BlockStructure_Normal::DATA_POS_OF_DELETE_IDENTITY = 0;

BlockStructure_Normal::BlockStructure_Normal(std::string& block)
    : BlockStructure(block, BlockStructure::eBlockType_Normal)
    , m_KeySectionSize(0)
    , m_DataSectionSize(0)
{
    size_t ratio = Config::Ins().normal_block_key_ratio_to_16();
    size_t blockSize = block.size();
    m_KeySectionSize = ( (blockSize >> 4) * ratio - HeadSize() );
    m_DataSectionSize = ( (blockSize >> 4) * (16 - ratio) );
}

bool BlockStructure_Normal::IsEnoughForData(const std::string& block, const std::string& key, const std::string& value) const
{
    return false;
}

void BlockStructure_Normal::AppendData(std::string& block, const std::string& key, const std::string& value)
{
}

size_t BlockStructure_Normal::KeySectionSize(void) const
{
    return m_KeySectionSize;
}

size_t BlockStructure_Normal::DataSectionSize(void) const
{
    return m_DataSectionSize;
}

size_t BlockStructure_Normal::GetKeyNeedLen(const std::string& key)
{
    assert_check(key.size() > 0, "BlockStructure_Normal::GetKeyNeedLen");
    return sizeof(NormalBlock::key_len_t) + sizeof(data_pos_t) + sizeof(NormalBlock::KEY_CHECK_NUM) + key.size();
}

size_t BlockStructure_Normal::GetValueNeedLen(const std::string& value)
{
    return sizeof(NormalBlock::data_len_t) + sizeof(NormalBlock::DATA_CHECK_NUM) + value.size();
}

Essential::_binary_buf BlockStructure_Normal::MakeKeyBuffer(const std::string& key, const data_pos_t& data_pos)
{
    assert_check(key.size() > 0, "BlockStructure_Normal::MakeKeyBuffer");

    Essential::_binary_buf buffer;
    Essential::_binary_ostream<Essential::_binary_buf> bfstream(buffer);
    bfstream.Pack<NormalBlock::key_len_t>(key.size());
    bfstream.Pack<data_pos_t>(data_pos);
    if (key.size() > 0)
        bfstream.Pack(key.c_str(), key.size());
    bfstream.Pack(NormalBlock::KEY_CHECK_NUM);
    return buffer;
}

Essential::_binary_buf BlockStructure_Normal::MakeDeletedKeyBuffer(const std::string& key)
{
    return MakeKeyBuffer(key, DATA_POS_OF_DELETE_IDENTITY);
}
