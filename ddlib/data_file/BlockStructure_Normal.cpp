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
        typedef uint16_t key_count_t;
        typedef uint16_t key_len_t;
        typedef uint16_t data_section_used_size_t;
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

bool BlockStructure_Normal::IsEnoughForData(const std::string& key, const std::string& value) const
{
    return IsEnoughForKeySection(key) && IsEnoughForDataSection(key, value);
}

bool BlockStructure_Normal::IsEnoughForKeySection(const std::string& key) const
{
    //  calc keySection body used count except input key
    size_t sum = KeySection_BodyUsedCount_Except(key);

    //  count bytes need of new key.
    sum += GetKeyNeedLen(key);
    //  check
    return sum <= KeySection_BodySize();
}

bool BlockStructure_Normal::IsEnoughForDataSection(const std::string& key, const std::string& value) const
{
    // calc dataSection body used count except input key
    size_t sum = DataSection_BodyUsedCount_Except(key);
    // count bytes need of new value.
    sum += GetValueNeedLen(value);
    // check
    return sum <= DataSection_BodySize();
}

size_t BlockStructure_Normal::KeySection_BodyUsedCount_Except(const std::string& key) const
{
    //  iterator m_KeyValues
    //      filter the same key in m_KeyValues
    //      count

    size_t sum = 0;
    for (key_value_map_t::const_iterator iter = m_KeyValues.begin(); iter != m_KeyValues.end(); ++iter)
    {
        const std::string& tmpkey = iter->first;
        if (key == tmpkey)
            continue;
        sum += GetKeyNeedLen(tmpkey);
    }
    return sum;
}

void BlockStructure_Normal::AddDataToKeyValueMap(const std::string& key, const std::string& value)
{
    m_KeyValues[key] = value;
}

size_t BlockStructure_Normal::DataSection_BodyUsedCount_Except(const std::string& key) const
{
    //  iterator m_KeyValues
    //      filter the same key in m_KeyValues
    //      count

    size_t sum = 0;
    for (key_value_map_t::const_iterator iter = m_KeyValues.begin(); iter != m_KeyValues.end(); ++iter)
    {
        const std::string& tmpkey = iter->first;
        const std::string& tmpvalue = iter->second;
        if (key == tmpkey)
            continue;
        sum += GetValueNeedLen(tmpvalue);
    }
    return sum;
}

void BlockStructure_Normal::AddData(const std::string& key, const std::string& value)
{
    // checking enough for data
    // if finding key in  m_KeyValues
    //     remove key/value from m_KeyValues
    //     remake the entire block
    // end
    // add to m_KeyValues
    // add to block
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

size_t BlockStructure_Normal::KeySection_BodySize(void) const
{
    return KeySectionSize() - sizeof(NormalBlock::key_count_t);
}

size_t BlockStructure_Normal::DataSection_BodySize(void) const
{
    return DataSectionSize() - sizeof(NormalBlock::data_section_used_size_t);
}
