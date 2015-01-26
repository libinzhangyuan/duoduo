// db_imp_test.cpp

#include <cppunit/config/SourcePrefix.h>
#include <binary_stream/binary_istream.h>
#include <data_file/BlockStructure.h>
#include "BlockStructure_NormalTest.h"
#include <Util.h>
#include <Config.h>
#include <check_function.h>

#define private public
#include <data_file/BlockStructure_Normal.h>
#undef private


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( BlockStructure_NormalTest );

using namespace DuoDuo;
using namespace Essential;

void BlockStructure_NormalTest::setUp()
{
}


void BlockStructure_NormalTest::tearDown()
{
}


void BlockStructure_NormalTest::test_constructor()
{
}

void BlockStructure_NormalTest::test_create()
{
}

void BlockStructure_NormalTest::test_InitBlock()
{
    std::string block;
    block.resize(4096);
    BlockStructure_Normal normal(block);
    normal.InitBlock();
    CPPUNIT_ASSERT(block.size() == Config::Ins().ssd_block_size());
    CPPUNIT_ASSERT(block[0] == '\0');
    CPPUNIT_ASSERT(block[1] == '\0');
    CPPUNIT_ASSERT(block[2] == BlockStructure::eBlockType_Normal);
    CPPUNIT_ASSERT(block[3] == '\0');
    CPPUNIT_ASSERT(block[4] == '\0');
}

void BlockStructure_NormalTest::test_KeySectionSize()
{
    std::string block;
    block.resize(4096);
    BlockStructure_Normal normal(block);
    CPPUNIT_ASSERT(normal.KeySectionSize() == 1276); // (4096 * 5 / 16) - 4
}

void BlockStructure_NormalTest::test_KeySection_BodySize()
{
    std::string block;
    block.resize(4096);
    BlockStructure_Normal normal(block);
    CPPUNIT_ASSERT(normal.KeySection_BodySize() == 1274);
}

void BlockStructure_NormalTest::test_DataSectionSize()
{
    std::string block;
    block.resize(4096);
    BlockStructure_Normal normal(block);
    CPPUNIT_ASSERT(normal.DataSectionSize() == 2816); // (4096 * 11 / 16)
}


void BlockStructure_NormalTest::test_DataSection_BodySize()
{
    std::string block;
    block.resize(4096);
    BlockStructure_Normal normal(block);
    CPPUNIT_ASSERT(normal.DataSection_BodySize() == 2814);
}

void BlockStructure_NormalTest::test_GetKeyNeedLen()
{
    CPPUNIT_ASSERT(BlockStructure_Normal::GetKeyNeedLen(std::string("1234567890")) == 15);
    CPPUNIT_ASSERT(BlockStructure_Normal::GetKeyNeedLen(std::string("1")) == 6);
    CPPUNIT_ASSERT_THROW(BlockStructure_Normal::GetKeyNeedLen(std::string("")), AssertException);
}

void BlockStructure_NormalTest::test_GetValueNeedLen()
{
    CPPUNIT_ASSERT(BlockStructure_Normal::GetValueNeedLen(std::string("1234567890")) == 13);
    CPPUNIT_ASSERT(BlockStructure_Normal::GetValueNeedLen(std::string("1")) == 4);
    CPPUNIT_ASSERT(BlockStructure_Normal::GetValueNeedLen(std::string("")) == 3);
}

void BlockStructure_NormalTest::test_MakeKeyBuffer()
{
    {
        Essential::_binary_buf buf = BlockStructure_Normal::MakeKeyBuffer("123", 12);
        Essential::_binary_istream<Essential::_binary_buf> istr(buf);
        CPPUNIT_ASSERT(istr.Unpack<uint16_t>() == 3);
        CPPUNIT_ASSERT(istr.Unpack<uint16_t>() == 12);
        CPPUNIT_ASSERT(istr.Unpack(3) == std::string("123"));
        CPPUNIT_ASSERT(istr.Unpack<char>() == char(0xF1));
    }

    {
        CPPUNIT_ASSERT_THROW(BlockStructure_Normal::MakeKeyBuffer("", 12), AssertException);
    }
}

void BlockStructure_NormalTest::test_MakeDeletedKeyBuffer()
{
    {
        Essential::_binary_buf buf = BlockStructure_Normal::MakeDeletedKeyBuffer("123");
        //printf("\nbuf=============: %s\n", buf.to_cstyle_str().c_str());
        Essential::_binary_istream<Essential::_binary_buf> istr(buf);
        CPPUNIT_ASSERT(istr.Unpack<uint16_t>() == 3);
        CPPUNIT_ASSERT(istr.Unpack<uint16_t>() == 0);
        CPPUNIT_ASSERT(istr.Unpack(3) == std::string("123"));
        CPPUNIT_ASSERT(istr.Unpack<char>() == char(0xF1));
    }

    {
        CPPUNIT_ASSERT_THROW(BlockStructure_Normal::MakeKeyBuffer("", 12), AssertException);
    }
}

void BlockStructure_NormalTest::test_IsEnoughForData()
{
    // test keySection
    {
        // normal
        {
            std::string block;
            block.resize(4096);
            BlockStructure_Normal normal(block);
            CPPUNIT_ASSERT(normal.IsEnoughForData("key23423", "value23423") == true);

            normal.AddDataToKeyValueMap("key12345", "value123456");
            CPPUNIT_ASSERT(normal.IsEnoughForData("key23423", "value23423") == true);
        }

        // key section sold out
        {
            std::string block;
            block.resize(64);
            BlockStructure_Normal normal(block);
            CPPUNIT_ASSERT(normal.KeySection_BodySize() == 14); // (64 * 5 / 16) - 4 - 2
            CPPUNIT_ASSERT(normal.DataSection_BodySize() == 42); // (64 * 11 / 16) - 2
            std::string key1 = "1k";
            std::string value1 = "1v34567890";
            size_t key1_needLen = BlockStructure_Normal::GetKeyNeedLen(key1);
            CPPUNIT_ASSERT(key1_needLen == 7);
            size_t value1_needLen = BlockStructure_Normal::GetValueNeedLen(value1);
            CPPUNIT_ASSERT(value1_needLen = 13);

            CPPUNIT_ASSERT(normal.IsEnoughForData(key1, value1) == true);
            normal.AddDataToKeyValueMap(key1, value1);

            // sold out
            {
                std::string key2 = "2k345";
                size_t key2_needLen = BlockStructure_Normal::GetKeyNeedLen(key2);
                CPPUNIT_ASSERT(key2_needLen == 10);
                std::string value2 = "2v34567890";
                CPPUNIT_ASSERT(normal.IsEnoughForData(key2, value2) == false);
            }

            // duplication in one normalBlock
            {
                CPPUNIT_ASSERT(normal.IsEnoughForData(key1, "new valuegasdfasdf") == true);
            }

            // in boundary
            {
                std::string key3 = "3k";
                size_t key3_needLen = BlockStructure_Normal::GetKeyNeedLen(key3);
                CPPUNIT_ASSERT(key3_needLen == 7);
                std::string value3 = "2v34567890";
                CPPUNIT_ASSERT(normal.IsEnoughForData(key3, value3) == true);
            }

            // out boundary
            {
                std::string key4 = "3k3";
                size_t key4_needLen = BlockStructure_Normal::GetKeyNeedLen(key4);
                CPPUNIT_ASSERT(key4_needLen == 8);
                std::string value4 = "2v34567890";
                CPPUNIT_ASSERT(normal.IsEnoughForData(key4, value4) == false);
            }
        }

        // key is empty
        {
            std::string block;
            block.resize(64);
            BlockStructure_Normal normal(block);

            CPPUNIT_ASSERT_THROW(normal.IsEnoughForData("", "fasdfasdfas"), AssertException);
        }

        // value is empty
        {
            std::string block;
            block.resize(64);
            BlockStructure_Normal normal(block);

            std::string key3 = "3k";
            size_t key3_needLen = BlockStructure_Normal::GetKeyNeedLen(key3);
            CPPUNIT_ASSERT(key3_needLen == 7);
            std::string value3 = "2v34567890";
            CPPUNIT_ASSERT(normal.IsEnoughForData(key3, "") == true);
        }

        // add data with delete identity

        // key too large for a empty normalBlock
        {
            std::string block;
            block.resize(64);
            BlockStructure_Normal normal(block);

            CPPUNIT_ASSERT(normal.IsEnoughForData("123456789", "gasdf") == true);
            CPPUNIT_ASSERT(normal.IsEnoughForData("1234567890", "fdgs") == false);
        }

    } // end of test keySection


    // test dataSection
    {
        // data section sold out
        {
            std::string block;
            block.resize(64);
            BlockStructure_Normal normal(block);
            CPPUNIT_ASSERT(normal.KeySection_BodySize() == 14); // (64 * 5 / 16) - 4 - 2
            CPPUNIT_ASSERT(normal.DataSection_BodySize() == 42); // (64 * 11 / 16) - 2
            std::string key1 = "1";
            std::string value1 = "1v34567890";
            size_t key1_needLen = BlockStructure_Normal::GetKeyNeedLen(key1);
            CPPUNIT_ASSERT(key1_needLen == 6);
            size_t value1_needLen = BlockStructure_Normal::GetValueNeedLen(value1);
            CPPUNIT_ASSERT(value1_needLen = 13);

            CPPUNIT_ASSERT(normal.IsEnoughForData(key1, value1) == true);
            normal.AddDataToKeyValueMap(key1, value1);
 
            // sold out
            {
                std::string key2 = "2";
                std::string value2 = "2v345678901234567890123456789012345";  // keyNeedLen = 38
                CPPUNIT_ASSERT(normal.IsEnoughForData(key2, value2) == false);
            }

            // duplication in one normalBlock
            {
                std::string key2 = "1";
                std::string value2 = "2v345678901234567890123456789012345";  // keyNeedLen = 38
                CPPUNIT_ASSERT(normal.IsEnoughForData(key2, value2) == true);
            }

            // in boundary
            {
                std::string key2 = "2";
                std::string value2 = "2v345678901234567890123456";  // keyNeedLen = 29
                CPPUNIT_ASSERT(normal.IsEnoughForData(key2, value2) == true);
            }

            // out boundary
            {
                std::string key2 = "2";
                std::string value2 = "2v3456789012345678901234567";  // keyNeedLen = 30
                CPPUNIT_ASSERT(normal.IsEnoughForData(key2, value2) == false);
            }
        }

        // value is empty
        {
            std::string block;
            block.resize(128);
            BlockStructure_Normal normal(block);
            CPPUNIT_ASSERT(normal.KeySection_BodySize() == 34); // (128 * 5 / 16) - 4 - 2
            CPPUNIT_ASSERT(normal.DataSection_BodySize() == 86); // (128 * 11 / 16) - 2

            std::string value1 = "1v34567890"; // value need len: 13
            normal.AddDataToKeyValueMap("1", value1);
            CPPUNIT_ASSERT(normal.IsEnoughForData("n", "") == true);

            std::string value2 = "123456789012345678901234567890"
                "123456789012345678901234567890"
                "1234567";  // value need len = 70
            CPPUNIT_ASSERT(normal.IsEnoughForData("n", value2) == true);
            normal.AddDataToKeyValueMap("2", value2);

            std::string value3 = ""; // value need len = 3
            CPPUNIT_ASSERT(normal.IsEnoughForData("n", "") == true);
            CPPUNIT_ASSERT(normal.IsEnoughForData("n", "1") == false);
        }

        // value too large for a empty normalBlock
        {
            std::string block;
            block.resize(64);
            BlockStructure_Normal normal(block);

            std::string key2 = "2";
            std::string value2 = "2v3456789012345678901234567890123456789";  // keyNeedLen = 42
            CPPUNIT_ASSERT(normal.IsEnoughForData(key2, value2) == true);
            std::string value3 = "2v34567890123456789012345678901234567890";  // keyNeedLen = 43
            CPPUNIT_ASSERT(normal.IsEnoughForData(key2, value3) == false);
        }

        // add data with delete identity


    } // end of test dataSection
}
