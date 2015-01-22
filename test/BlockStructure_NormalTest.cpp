// db_imp_test.cpp

#include <cppunit/config/SourcePrefix.h>
#include <data_file/BlockStructure.h>
#include "BlockStructure_NormalTest.h"

#define private public
#include <data_file/BlockStructure_Normal.h>
#undef private

#include <Util.h>
#include <Config.h>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( BlockStructure_NormalTest );

using namespace DuoDuo;

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
