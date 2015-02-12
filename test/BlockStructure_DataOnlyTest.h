// DataFileTest.h
#ifndef _BLOCK_STRUCTURE_DATA_ONLY_TEST_H_
#define _BLOCK_STRUCTURE_DATA_ONLY_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

class BlockStructure_DataOnlyTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( BlockStructure_DataOnlyTest );
  CPPUNIT_TEST( test_constructor );
  CPPUNIT_TEST( test_BlockNeedCount );
  CPPUNIT_TEST( test_SaveAndLoad );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void test_constructor();
  void test_BlockNeedCount();
  void test_SaveAndLoad();
};

#endif  // _BLOCK_STRUCTURE_DATA_ONLY_TEST_H_
