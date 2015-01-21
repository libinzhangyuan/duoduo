// db_imp_test.cpp

#include <cppunit/config/SourcePrefix.h>
#include <data_file/BlockStructure.h>
#include "BlockStructureTest.h"
#include <Util.h>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( BlockStructureTest );

using namespace DuoDuo;

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
}

void BlockStructureTest::test_SelectStructure_by_blockType(void)
{
    CPPUNIT_ASSERT(BlockStructure::SelectStructure(BlockStructure::eBlockType_Normal, 4096) != NULL);
    CPPUNIT_ASSERT(BlockStructure::SelectStructure(BlockStructure::eBlockType_Normal, 4096)->Type() == BlockStructure::eBlockType_Normal);

    CPPUNIT_ASSERT(BlockStructure::SelectStructure(BlockStructure::eBlockType_Big, 4096) != NULL);
    CPPUNIT_ASSERT(BlockStructure::SelectStructure(BlockStructure::eBlockType_Big, 4096)->Type() == BlockStructure::eBlockType_Big);

    CPPUNIT_ASSERT(BlockStructure::SelectStructure(BlockStructure::eBlockType_DataOnly, 4096) != NULL);
    CPPUNIT_ASSERT(BlockStructure::SelectStructure(BlockStructure::eBlockType_DataOnly, 4096)->Type() == BlockStructure::eBlockType_DataOnly);

    CPPUNIT_ASSERT_THROW(BlockStructure::SelectStructure(BlockStructure::eBlockType_None, 4096), AssertException);
    //CPPUNIT_ASSERT(BlockStructure::SelectStructure(BlockStructure::eBlockType_None, 4096) == NULL); // assert
}
