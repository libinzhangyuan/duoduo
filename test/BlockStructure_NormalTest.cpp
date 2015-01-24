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

void BlockStructure_NormalTest::test_DataSectionSize()
{
    std::string block;
    block.resize(4096);
    BlockStructure_Normal normal(block);
    CPPUNIT_ASSERT(normal.DataSectionSize() == 2816); // (4096 * 11 / 16)
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
