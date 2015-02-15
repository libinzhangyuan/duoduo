// DataBlockTest.h
#ifndef _DATA_BLOCK_TEST_H_
#define _DATA_BLOCK_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

class DataBlockTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( DataBlockTest );
  CPPUNIT_TEST( test_constructor );
  CPPUNIT_TEST( test_create );
  CPPUNIT_TEST( test_assignment );
  CPPUNIT_TEST( test_GetExtraBlockCount );
  CPPUNIT_TEST( test_CreateDataAndGetData );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void test_constructor();
  void test_create();
  void test_assignment();
  void test_GetExtraBlockCount();
  void test_CreateDataAndGetData();
};

#endif  // _DATA_BLOCK_TEST_H_
