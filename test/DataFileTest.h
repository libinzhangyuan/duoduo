// DataFileTest.h
#ifndef DATAFILETEST_H
#define DATAFILETEST_H

#include <cppunit/extensions/HelperMacros.h>

class DataFileTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( DataFileTest );
  CPPUNIT_TEST( test_constructor );
  CPPUNIT_TEST( test_create );
  CPPUNIT_TEST( test_add_data );
  CPPUNIT_TEST( test_get_data );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void test_constructor();
  void test_create();
  void test_add_data();
  void test_get_data();
};

#endif  // DATAFILETEST_H