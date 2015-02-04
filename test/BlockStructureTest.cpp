// db_imp_test.cpp

#include <cppunit/config/SourcePrefix.h>
#include "BlockStructureTest.h"
#include <Util.h>
#include <check_function.h>
#include "test_def.h"

#define private public
#define protected public
#include <block/BlockStructure.h>
#undef private
#undef protected

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( BlockStructureTest );


using namespace DuoDuo;
using namespace Essential;

void BlockStructureTest::setUp()
{
}


void BlockStructureTest::tearDown()
{
}


void BlockStructureTest::test_constructor()
{
}

void BlockStructureTest::test_create()
{
    block_t block;
    block.resize(4096);
    CPPUNIT_ASSERT(BlockStructure::Create(block, BlockStructure::eBlockType_Normal) != NULL);
    CPPUNIT_ASSERT(BlockStructure::Create(block, BlockStructure::eBlockType_Normal)->Type() == BlockStructure::eBlockType_Normal);

    CPPUNIT_ASSERT(BlockStructure::Create(block, BlockStructure::eBlockType_Big) != NULL);
    CPPUNIT_ASSERT(BlockStructure::Create(block, BlockStructure::eBlockType_Big)->Type() == BlockStructure::eBlockType_Big);

    CPPUNIT_ASSERT(BlockStructure::Create(block, BlockStructure::eBlockType_DataOnly) != NULL);
    CPPUNIT_ASSERT(BlockStructure::Create(block, BlockStructure::eBlockType_DataOnly)->Type() == BlockStructure::eBlockType_DataOnly);

    CPPUNIT_ASSERT_THROW(BlockStructure::Create(block, BlockStructure::eBlockType_None), AssertException);
}

void BlockStructureTest::test_CleanBody()
{
    block_t block;
    block.resize(4096);
    BlockStructure* pBlockStruct = BlockStructure::Create(block, BlockStructure::eBlockType_Normal);
    CPPUNIT_ASSERT(pBlockStruct != NULL);
    block[1024] = '2';
    CPPUNIT_ASSERT(block[1024] == '2');
    pBlockStruct->CleanBody();
    CPPUNIT_ASSERT(block[1024] == 0);
}


/*
void BlockStructureTest::test_SelectStructure_by_blockType(void)
{
    CPPUNIT_ASSERT(BlockStructure::SelectStructure(BlockStructure::eBlockType_Normal, 4096) != NULL);
    CPPUNIT_ASSERT(BlockStructure::SelectStructure(BlockStructure::eBlockType_Normal, 4096)->Type() == BlockStructure::eBlockType_Normal);

    CPPUNIT_ASSERT(BlockStructure::SelectStructure(BlockStructure::eBlockType_Big, 4096) != NULL);
    CPPUNIT_ASSERT(BlockStructure::SelectStructure(BlockStructure::eBlockType_Big, 4096)->Type() == BlockStructure::eBlockType_Big);

    CPPUNIT_ASSERT(BlockStructure::SelectStructure(BlockStructure::eBlockType_DataOnly, 4096) != NULL);
    CPPUNIT_ASSERT(BlockStructure::SelectStructure(BlockStructure::eBlockType_DataOnly, 4096)->Type() == BlockStructure::eBlockType_DataOnly);

    CPPUNIT_ASSERT_THROW(BlockStructure::SelectStructure(BlockStructure::eBlockType_None, 4096), AssertException);
}
*/
