// DataFileTest.h
#ifndef _BLOCK_STRUCTURE_BIG_TEST_H_
#define _BLOCK_STRUCTURE_BIG_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

class BlockStructure_BigTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( BlockStructure_BigTest );
  CPPUNIT_TEST( test_constructor );
  CPPUNIT_TEST( test_IsEnoughForData );
  CPPUNIT_TEST( test_ValueSize_CanStoreToBlock );
  CPPUNIT_TEST( test_AddData );
  CPPUNIT_TEST( test_PackBlock );
  CPPUNIT_TEST( test_LoadFromBlock );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void test_constructor();
  void test_IsEnoughForData();
  void test_ValueSize_CanStoreToBlock();
  void test_AddData();
  void test_PackBlock();
  void test_LoadFromBlock();
};

#endif  // _BLOCK_STRUCTURE_BIG_TEST_H_
