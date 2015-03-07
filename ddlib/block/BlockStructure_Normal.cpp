#include "../Config.h"
#include "../def.h"

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
        typedef uint16_t data_body_section_used_count_t;
        typedef uint16_t data_len_t;
        typedef char key_check_num_t;
        typedef char data_check_num_t;

        const char KEY_CHECK_NUM = char(0xF1);
        const char DATA_CHECK_NUM = char(0xF2);
    }

}

namespace DuoDuo // StructCalc
{
    size_t BlockStructure_Normal::StructCalc::HeadSize(void) const
    {
        return DD_BLOCK_HEAD_SIZE;
    }

    size_t BlockStructure_Normal::StructCalc::DataSection_BodySize(void) const
    {
        return DataSectionSize() - sizeof(NormalBlock::data_body_section_used_count_t);
    }

    pos_in_block_t BlockStructure_Normal::StructCalc::DataSection_BodyStartPos(void) const
    {
        return HeadSize() + KeySectionSize() + DataSectionHeadSize();
    }

    size_t BlockStructure_Normal::StructCalc::KeySection_BodySize(void) const
    {
        return KeySectionSize() - sizeof(NormalBlock::key_count_t);
    }

    size_t BlockStructure_Normal::StructCalc::KeySectionSize(void) const
    {
        size_t ratio = Config::Ins().normal_block_key_ratio_to_16();
        return (m_BlockSize >> 4) * ratio - HeadSize();
    }

    size_t BlockStructure_Normal::StructCalc::DataSectionSize(void) const
    {
        size_t ratio = Config::Ins().normal_block_key_ratio_to_16();
        return (m_BlockSize >> 4) * (16 - ratio);
    }

    size_t BlockStructure_Normal::StructCalc::DataSectionHeadSize(void) const
    {
        return sizeof(NormalBlock::data_body_section_used_count_t);
    }

    size_t BlockStructure_Normal::StructCalc::GetKeyNeedLen(const std::string& key) const
    {
        assert_check(key.size() > 0, "BlockStructure_Normal::StructCalc::GetKeyNeedLen");
        return sizeof(NormalBlock::key_len_t) + sizeof(data_pos_t) + sizeof(NormalBlock::KEY_CHECK_NUM) + key.size();
    }

    size_t BlockStructure_Normal::StructCalc::GetValueNeedLen(const std::string& value) const
    {
        return sizeof(NormalBlock::data_len_t) + sizeof(NormalBlock::DATA_CHECK_NUM) + value.size();
    }

    bool BlockStructure_Normal::StructCalc::IsNormalData(const std::string& key, const std::string& value) const
    {
        return IsEnoughForKeySection(key) && IsEnoughForDataSection(key, value);
    }

    bool BlockStructure_Normal::StructCalc::IsEnoughForKeySection(const std::string& key) const
    {
        return GetKeyNeedLen(key) <= KeySection_BodySize();
    }

    bool BlockStructure_Normal::StructCalc::IsEnoughForDataSection(const std::string& key, const std::string& value) const
    {
        return GetValueNeedLen(value) <= DataSection_BodySize();
    }

}



//Setting data pos to 0 to indicating this 'key/value' is deleted.
const BlockStructure_Normal::data_pos_t BlockStructure_Normal::DATA_POS_OF_DELETE_IDENTITY = 0;

BlockStructure_Normal::BlockStructure_Normal(block_t& block)
    : BlockStructure(block, BlockStructure::eBlockType_Normal)
    , m_StructCalc(block.size())
{
}

BlockStructure* BlockStructure_Normal::Clone(block_t& blockNewStructureBindingTo)
{
    BlockStructure_Normal* pClone = new BlockStructure_Normal(blockNewStructureBindingTo);
    pClone->m_BlockType = m_BlockType;
    pClone->m_BlockSize = m_BlockSize;
    pClone->m_KeyValues = m_KeyValues;
    pClone->m_StructCalc = m_StructCalc;
    return pClone;
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
    sum += StructCalc(BlockSize()).GetKeyNeedLen(key);
    //  check
    return sum <= StructCalc(BlockSize()).KeySection_BodySize();
}

bool BlockStructure_Normal::IsEnoughForDataSection(const std::string& key, const std::string& value) const
{
    // calc dataSection body used count except input key
    size_t sum = DataSection_BodyUsedCount_Except(key);
    // count bytes need of new value.
    sum += StructCalc(BlockSize()).GetValueNeedLen(value);
    // check
    return sum <= StructCalc(BlockSize()).DataSection_BodySize();
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
        sum += StructCalc(BlockSize()).GetKeyNeedLen(tmpkey);
    }
    return sum;
}

void BlockStructure_Normal::LoadFromBlock(void)
{
    // check head
    assert_check(GetBlockTypeFromBlock() == BlockStructure::eBlockType_Normal, "BlockStructure_Normal::LoadFromBlock : check head");

    Essential::_binary_istream<Essential::_binary_buf> key_section_stream(GetBlock());

    // key_section_stream:  move head
    key_section_stream.MoveReadPos(HeadSize());

    // key count
    const NormalBlock::key_count_t key_count = key_section_stream.Unpack<NormalBlock::key_count_t>();

    // each key and data
    for (size_t i = 0; i < key_count; ++i)
    {
        data_pos_t data_pos;
        const std::string& key = GetKey(key_section_stream, data_pos);
        exception_assert(data_pos >= StructCalc(BlockSize()).DataSection_BodyStartPos(), "NormalBlock Load: data_pos too small");
        // todo: data_pos == 0 means this key is deleted.
        const std::string& value = GetValue(data_pos);
        AddDataToKeyValueMap(key, value);
    }
}

std::string BlockStructure_Normal::GetKey(Essential::_binary_istream<Essential::_binary_buf>& key_stream, data_pos_t& data_pos)
{
    // key len
    exception_assert(sizeof(NormalBlock::key_len_t) < key_stream.LeftBytes(), "NormalBlock GetKey: keyLen out of range");
    const NormalBlock::key_len_t key_length = key_stream.Unpack<NormalBlock::key_len_t>();
    exception_assert(key_length > 0, "NormalBlock Load: key length must > 0");

    // data pos
    exception_assert(sizeof(data_pos_t) < key_stream.LeftBytes(), "NormalBlock GetKey: dataPos out of range");
    data_pos = key_stream.Unpack<data_pos_t>();

    // key
    exception_assert(key_length < key_stream.LeftBytes(), "NormalBlock GetKey: key out of range");
    const std::string& key = key_stream.Unpack(key_length);

    // check num
    exception_assert(sizeof(char) <= key_stream.LeftBytes(), "NormalBlock GetKey: cknum out of range");
    const char cknum = key_stream.Unpack<char>();
    exception_assert(cknum == NormalBlock::KEY_CHECK_NUM, "NormalBlock Load: keyCkNum wrong!");

    return key;
}

std::string BlockStructure_Normal::GetValue(Essential::_binary_istream<Essential::_binary_buf>& value_stream)
{
    // value len
    exception_assert(sizeof(NormalBlock::data_len_t) < value_stream.LeftBytes(), "NormalBlock GetValue: valueLen out of range");
    const NormalBlock::data_len_t value_len = value_stream.Unpack<NormalBlock::data_len_t>();
    exception_assert(value_len < value_stream.LeftBytes(), "NormalBlock Load: data len too large");

    // value
    const std::string& value = value_stream.Unpack(value_len);

    // check num
    const char value_cknum = value_stream.Unpack<char>();
    exception_assert(value_cknum == NormalBlock::DATA_CHECK_NUM, "NormalBlock Load: valueCkNum wrong!");

    return value;
}

std::string BlockStructure_Normal::GetValue(const data_pos_t& data_pos) const
{
    Essential::_binary_istream<Essential::_binary_buf> data_body_stream(GetBlock());

    exception_assert((data_pos + sizeof(NormalBlock::data_len_t)) < GetBlock().size(), "NormalBlock get value: value len out of range!");
    data_body_stream.SetReadPos(data_pos);

    return DuoDuo::BlockStructure_Normal::GetValue(data_body_stream);
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
        sum += StructCalc(BlockSize()).GetValueNeedLen(tmpvalue);
    }
    return sum;
}

void BlockStructure_Normal::AddData(const std::string& key, const std::string& value)
{
    // checking enough for data
    // if finding key in  m_KeyValues
    //     change key/value to new one
    //     remake the entire block
    //     return
    // end
    // add to m_KeyValues
    // add to block


    // checking enough for data
    assert_check(IsEnoughForData(key, value), "BlockStructure_Normal::AddData");

    // if finding key in  m_KeyValues
    //     change key/value to new one
    //     remake the entire block
    //     return
    // end
    if (m_KeyValues.find(key) != m_KeyValues.end())
    {
        AddDataToKeyValueMap(key, value);
        PackBlock();
        return;
    }

    // add to m_KeyValues
    AddDataToKeyValueMap(key, value);
    // add to block
    PackBlock(); // todo: change code to append new key && value.
    return;
}

void BlockStructure_Normal::PackBlock(void)
{
    CleanBody();

    Essential::_binary_ostream<Essential::_binary_buf> bfstream(m_Block);

    // head
    InitHead();
    bfstream.SetWritePos(HeadSize());

    // key count
    bfstream.Pack<NormalBlock::key_count_t>(m_KeyValues.size());

    // key section body
    const KeyDataBodyResult& key_and_data_body = MakeKeySectionBodyAndDataSectionBody(m_KeyValues);
    assert_check(key_and_data_body.key_section_body.size() <= m_StructCalc.KeySection_BodySize(), "BlockStructure_Normal::PackBlock");
    bfstream.Pack(key_and_data_body.key_section_body.c_str(), key_and_data_body.key_section_body.size());

    // data body section used count
    bfstream.Pack<NormalBlock::data_body_section_used_count_t>(key_and_data_body.data_section_body.size());

    // data section body
    assert_check(key_and_data_body.data_section_body.size() <= m_StructCalc.DataSection_BodySize(), "BlockStructure_Normal::PackBlock");
    bfstream.Pack(key_and_data_body.data_section_body.c_str(), key_and_data_body.data_section_body.size());
}

BlockStructure_Normal::KeyDataBodyResult BlockStructure_Normal::MakeKeySectionBodyAndDataSectionBody(const key_value_map_t& key_values) const
{
    BlockStructure_Normal::KeyDataBodyResult result;
    Essential::_binary_ostream<Essential::_binary_buf> key_body_stream(result.key_section_body);
    Essential::_binary_ostream<Essential::_binary_buf> data_body_stream(result.data_section_body);
    for (key_value_map_t::const_iterator iter = key_values.begin(); iter != key_values.end(); ++iter)
    {
        const std::string& tmpkey = iter->first;
        const std::string& tmpvalue = iter->second;
        const unsigned int tmpvalue_start_pos = data_body_stream.GetWritePos();
        data_pos_t tmpvalue_pos_in_block = StructCalc(BlockSize()).DataSection_BodyStartPos() + tmpvalue_start_pos;

        // pack key
        const Essential::_binary_buf& keyBuff = MakeKeyBuffer(tmpkey, tmpvalue_pos_in_block);
        key_body_stream.Pack(keyBuff.c_str(), keyBuff.size());

        // pack value
        const Essential::_binary_buf& valueBuff = MakeValueBuffer(tmpvalue);
        data_body_stream.Pack(valueBuff.c_str(), valueBuff.size());
    }
    return result;
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

Essential::_binary_buf BlockStructure_Normal::MakeValueBuffer(const std::string& value)
{
    Essential::_binary_buf buffer;
    Essential::_binary_ostream<Essential::_binary_buf> bfstream(buffer);
    bfstream.Pack<NormalBlock::data_len_t>(value.size());
    bfstream.Pack(value.c_str(), value.size());
    bfstream.Pack<char>(NormalBlock::DATA_CHECK_NUM);
    return buffer;
}

std::map<std::string /*key*/, pos_in_block_t> BlockStructure_Normal::IndexFromBlock(void) const
{
    std::map<std::string /*key*/, pos_in_block_t> index;

    // check head
    assert_check(GetBlockTypeFromBlock() == BlockStructure::eBlockType_Normal, "BlockStructure_Normal::IndexFromBlock : check head");

    Essential::_binary_istream<Essential::_binary_buf> key_section_stream(GetBlock());

    // key_section_stream:  move head
    key_section_stream.MoveReadPos(HeadSize());

    // key count
    const NormalBlock::key_count_t key_count = key_section_stream.Unpack<NormalBlock::key_count_t>();

    // each key
    for (size_t i = 0; i < key_count; ++i)
    {
        data_pos_t data_pos;
        const std::string& key = GetKey(key_section_stream, data_pos);
        exception_assert(data_pos >= StructCalc(BlockSize()).DataSection_BodyStartPos(), "NormalBlock Load: data_pos too small");
        // todo: data_pos == 0 means this key is deleted.

        index[key] = data_pos;
    }
    return index;
}

size_t BlockStructure_Normal::GetExtraBlockCount(void) const
{
    return 0;
}

const key_value_map_t& BlockStructure_Normal::GetDatas(void) const
{
    return m_KeyValues;
}
