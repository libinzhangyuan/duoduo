// DataFileTest.h
#ifndef DATAFILETEST_H
#define DATAFILETEST_H

#include <cppunit/extensions/HelperMacros.h>

class DataFileTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( DataFileTest );
  CPPUNIT_TEST( test_constructor );
  CPPUNIT_TEST( test_create );
  CPPUNIT_TEST( test_AddData );
  CPPUNIT_TEST( test_HasSmallData );
  CPPUNIT_TEST( test_HasBigData );
  CPPUNIT_TEST( test_PopBigBlock );
  CPPUNIT_TEST( test_PopSmallBlock );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void test_constructor();
  void test_create();
  void test_AddData();
  void test_HasSmallData();
  void test_HasBigData();
  void test_PopBigBlock();
  void test_PopSmallBlock();
};

#endif  // DATAFILETEST_H
