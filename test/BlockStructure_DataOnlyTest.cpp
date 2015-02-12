// db_imp_test.cpp

#include <cppunit/config/SourcePrefix.h>
#include <binary_stream/binary_istream.h>
#include <binary_stream/binary_ostream.h>
#include "BlockStructure_DataOnlyTest.h"
#include <Util.h>
#include <Config.h>
#include <check_function.h>
#include <def.h>
#include "test_def.h"
#define private public
#define protected public
#include <block/BlockStructure.h>
#include <block/BlockStructure_DataOnly.h>
#undef private
#undef protected


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( BlockStructure_DataOnlyTest );

using namespace DuoDuo;
using namespace Essential;

void BlockStructure_DataOnlyTest::setUp()
{
}


void BlockStructure_DataOnlyTest::tearDown()
{
}


void BlockStructure_DataOnlyTest::test_constructor()
{
}

void BlockStructure_DataOnlyTest::test_BlockNeedCount()
{
    BlockStructure_DataOnly::StructCalc calc(64);
    CPPUNIT_ASSERT(calc.MaxValueLen() == 46); // 64 - 16 - 2;
    CPPUNIT_ASSERT(calc.BlockNeedCount(0) == 0);
    CPPUNIT_ASSERT(calc.BlockNeedCount(1) == 1);
    CPPUNIT_ASSERT(calc.BlockNeedCount(11) == 1);
    CPPUNIT_ASSERT(calc.BlockNeedCount(45) == 1);
    CPPUNIT_ASSERT(calc.BlockNeedCount(46) == 1);
    CPPUNIT_ASSERT(calc.BlockNeedCount(47) == 2);
    CPPUNIT_ASSERT(calc.BlockNeedCount(46 * 20) == 20);
    CPPUNIT_ASSERT(calc.BlockNeedCount(46 * 2000 - 1) == 2000);
    CPPUNIT_ASSERT(calc.BlockNeedCount(46 * 20000 + 1) == 20001);
}

void BlockStructure_DataOnlyTest::test_SaveAndLoad()
{
    // normal
    {
        block_t block;
        block.resize(64);
        BlockStructure_DataOnly dataonly(block);
        dataonly.AddData("1234567890", "1234567890123456789012345678901234567890123456");
        CPPUNIT_ASSERT(dataonly.GetValueStoredInBlock() == std::string("1234567890123456789012345678901234567890123456"));
        dataonly.PackBlock();

        BlockStructure_DataOnly dataonly2(block);
        dataonly2.LoadFromBlock();
        CPPUNIT_ASSERT(dataonly2.GetValueStoredInBlock() == std::string("1234567890123456789012345678901234567890123456"));
    }

    // out bound
    {
        block_t block;
        block.resize(64);
        BlockStructure_DataOnly dataonly(block);
        dataonly.AddData("1234567890", "12345678901234567890123456789012345678901234567");
        CPPUNIT_ASSERT(dataonly.GetValueStoredInBlock() == std::string("1234567890123456789012345678901234567890123456"));
        dataonly.PackBlock();

        BlockStructure_DataOnly dataonly2(block);
        dataonly2.LoadFromBlock();
        CPPUNIT_ASSERT(dataonly2.GetValueStoredInBlock() == "1234567890123456789012345678901234567890123456");
    }


    // blank value
    {
        block_t block;
        block.resize(64);
        BlockStructure_DataOnly dataonly(block);
        CPPUNIT_ASSERT_THROW(dataonly.AddData("12345678901234567890", ""), Essential::AssertException);
    }
}
