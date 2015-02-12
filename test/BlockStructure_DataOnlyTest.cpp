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
    CPPUNIT_ASSERT(calc.MaxValueLen() == 48); // 64 - 16;
    CPPUNIT_ASSERT(calc.BlockNeedCount(0) == 0);
    CPPUNIT_ASSERT(calc.BlockNeedCount(1) == 1);
    CPPUNIT_ASSERT(calc.BlockNeedCount(11) == 1);
    CPPUNIT_ASSERT(calc.BlockNeedCount(47) == 1);
    CPPUNIT_ASSERT(calc.BlockNeedCount(48) == 1);
    CPPUNIT_ASSERT(calc.BlockNeedCount(49) == 2);
    CPPUNIT_ASSERT(calc.BlockNeedCount(48 * 20) == 20);
    CPPUNIT_ASSERT(calc.BlockNeedCount(48 * 2000 - 1) == 2000);
    CPPUNIT_ASSERT(calc.BlockNeedCount(48 * 20000 + 1) == 20001);
}
