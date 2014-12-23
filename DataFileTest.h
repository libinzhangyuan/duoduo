// DataFileTest.h
#ifndef DATAFILETEST_H
#define DATAFILETEST_H

#include <cppunit/extensions/HelperMacros.h>

class DataFileTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( DataFileTest );
  CPPUNIT_TEST( test_constructor );
  CPPUNIT_TEST( test_create );
  //CPPUNIT_TEST( testAdd );
  //CPPUNIT_TEST_EXCEPTION( testAddThrow, IncompatibleMoneyError );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void test_constructor();
  void test_create();
  //void testAdd();
  //void testAddThrow();
};

#endif  // DATAFILETEST_H
