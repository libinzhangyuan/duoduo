// DataFileTest.h
#ifndef _BLOCK_STRUCTURE_TEST_H_
#define _BLOCK_STRUCTURE_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

class BlockStructureTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( BlockStructureTest );
  CPPUNIT_TEST( test_constructor );
  CPPUNIT_TEST( test_create );
  //CPPUNIT_TEST( test_SelectStructure_by_blockType );
  CPPUNIT_TEST( test_CleanBody );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void test_constructor();
  void test_create();
  //void test_SelectStructure_by_blockType();
  void test_CleanBody();
};

#endif  // _BLOCK_STRUCTURE_TEST_H_
