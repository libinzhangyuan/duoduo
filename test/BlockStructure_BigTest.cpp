// db_imp_test.cpp

#include <cppunit/config/SourcePrefix.h>
#include <binary_stream/binary_istream.h>
#include <binary_stream/binary_ostream.h>
#include "BlockStructure_BigTest.h"
#include <Util.h>
#include <Config.h>
#include <check_function.h>
#include <def.h>
#include "test_def.h"
#define private public
#define protected public
#include <block/BlockStructure.h>
#include <block/BlockStructure_Big.h>
#undef private
#undef protected


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( BlockStructure_BigTest );

using namespace DuoDuo;
using namespace Essential;

void BlockStructure_BigTest::setUp()
{
}


void BlockStructure_BigTest::tearDown()
{
}


void BlockStructure_BigTest::test_constructor()
{
}

void BlockStructure_BigTest::test_MaxValueLen()
{
    BlockStructure_Big::StructCalc calc(64); // valueSize when keySize=10:  64 - 16 - 2 - 10 - 1 - 4 - 2 = 29
    CPPUNIT_ASSERT(calc.MaxValueLen(10) == 29);
    CPPUNIT_ASSERT(calc.MaxValueLen(39) == 0);
    CPPUNIT_ASSERT_CHECKING_THROW(calc.MaxValueLen(40));
}

void BlockStructure_BigTest::test_IsEnoughForData()
{
    block_t block;
    block.resize(64); // valueSize when keySize=10:  64 - 16 - 2 - 10 - 1 - 4 - 2 = 29
    BlockStructure_Big big(block);
    CPPUNIT_ASSERT(big.IsEnoughForData("1234567890", "12345678901234567890123456789") == true);
    CPPUNIT_ASSERT(big.IsEnoughForData("1234567890", "123456789012345678901234567890") == false);
    CPPUNIT_ASSERT(big.IsEnoughForData("12345678901", "12345678901234567890123456789") == false);
}

void BlockStructure_BigTest::test_ValueSize_CanStoreToBlock()
{
    {
        block_t block;
        block.resize(4096);
        BlockStructure_Big big(block);
        //                                                                                 key      dataLen
        CPPUNIT_ASSERT(big.m_StructCalc.MaxValueLen(10) == 4096 - DD_BLOCK_HEAD_SIZE - 2 - 10 - 1 -   4   -   2);
        //                                                                head         keyLen   chenNum    extraBlockCount 
    }

    {
        block_t block;
        block.resize(64);
        BlockStructure_Big big(block);
        //                                                                                       key      dataLen
        CPPUNIT_ASSERT(big.m_StructCalc.MaxValueLen(10) == 29); // 64 - DD_BLOCK_HEAD_SIZE - 2 - 10 - 1 -   4   -   2);
        //                                                                      head         keyLen   chenNum    extraBlockCount 
    }


}


void BlockStructure_BigTest::test_AddData()
{
    {
        block_t block;
        block.resize(64);
        BlockStructure_Big big(block);
        big.AddData("1234567890", "12345678901234567890123456789");
        CPPUNIT_ASSERT(big.m_Key == std::string("1234567890"));
        CPPUNIT_ASSERT(big.m_ValueTotalLen == 29);
        CPPUNIT_ASSERT(big.m_ValueStoredInBlock == std::string("12345678901234567890123456789"));
    }

    // out bound
    {
        block_t block;
        block.resize(64);
        BlockStructure_Big big(block);
        big.AddData("1234567890", "123456789012345678901234567890");
        CPPUNIT_ASSERT(big.m_Key == std::string("1234567890"));
        CPPUNIT_ASSERT(big.m_ValueTotalLen == 30);
        CPPUNIT_ASSERT(big.m_ValueStoredInBlock == std::string("12345678901234567890123456789"));
    }

    // blank value
    {
        block_t block;
        block.resize(64);
        BlockStructure_Big big(block);
        big.AddData("12345678901234567890", "");
        big.PackBlock();
        CPPUNIT_ASSERT(big.m_Key == std::string("12345678901234567890"));
        CPPUNIT_ASSERT(big.m_ValueTotalLen == 0);
        CPPUNIT_ASSERT(big.m_ValueStoredInBlock == std::string(""));
    }


    // blank key
    {
        block_t block;
        block.resize(64);
        BlockStructure_Big big(block);
        CPPUNIT_ASSERT_THROW(big.AddData("", "123456789012345678901234567890"), Essential::AssertException);
    }
}

void BlockStructure_BigTest::test_PackBlock()
{
    // normal
    {
        block_t block;
        block.resize(64);
        BlockStructure_Big big(block);
        big.AddData("1234567890", "12345678901234567890123456789");
        big.PackBlock();
        CPPUNIT_ASSERT(big.m_Block[2] == BlockStructure::eBlockType_Big);
        CPPUNIT_ASSERT(big.m_Block[63] == '9');
    }

    // out bound
    {
        block_t block;
        block.resize(64);
        BlockStructure_Big big(block);
        big.AddData("1234567890", "123456789012345678901234567890");
        big.PackBlock();
        CPPUNIT_ASSERT(big.m_Block[2] == BlockStructure::eBlockType_Big);
        CPPUNIT_ASSERT(big.m_Block[63] == '9');
    }

    // blank value
    {
        block_t block;
        block.resize(64);
        BlockStructure_Big big(block);
        big.AddData("12345678901234567890", "");
        big.PackBlock();
        CPPUNIT_ASSERT(big.m_Block[2] == BlockStructure::eBlockType_Big);
        CPPUNIT_ASSERT(big.m_Block[63] == 0);
    }

    // blank block
    {
        block_t block;
        block.resize(64);
        BlockStructure_Big big(block);
        CPPUNIT_ASSERT_THROW(big.PackBlock(), Essential::Exception);
    }
}

void BlockStructure_BigTest::test_LoadFromBlock()
{
    // normal
    {
        block_t block;
        block.resize(64);
        BlockStructure_Big big(block);
        big.AddData("1234567890", "12345678901234567890123456789");
        big.PackBlock();

        BlockStructure_Big big2(block);
        big2.LoadFromBlock();
        CPPUNIT_ASSERT(big2.m_Key == "1234567890");
        CPPUNIT_ASSERT(big2.m_ValueStoredInBlock == "12345678901234567890123456789");
        CPPUNIT_ASSERT(big2.m_ValueTotalLen == 29);
    }

    // out bound
    {
        block_t block;
        block.resize(64);
        BlockStructure_Big big(block);
        big.AddData("1234567890", "123456789012345678901234567890");
        big.PackBlock();

        BlockStructure_Big big2(block);
        big2.LoadFromBlock();
        CPPUNIT_ASSERT(big2.m_Key == "1234567890");
        CPPUNIT_ASSERT(big2.m_ValueStoredInBlock == "12345678901234567890123456789");
        CPPUNIT_ASSERT(big2.m_ValueTotalLen == 30);
    }


    // blank value
    {
        block_t block;
        block.resize(64);
        BlockStructure_Big big(block);
        big.AddData("12345678901234567890", "");
        big.PackBlock();

        BlockStructure_Big big2(block);
        big2.LoadFromBlock();
        CPPUNIT_ASSERT(big2.m_Key == "12345678901234567890");
        CPPUNIT_ASSERT(big2.m_ValueStoredInBlock == "");
        CPPUNIT_ASSERT(big2.m_ValueTotalLen == 0);
    }
}



