// DataFileTest.h
#ifndef _BLOCK_STRUCTURE_TEST_H_
#define _BLOCK_STRUCTURE_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

class BlockStructure_NormalTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( BlockStructure_NormalTest );
  CPPUNIT_TEST( test_constructor );
  CPPUNIT_TEST( test_create );
  CPPUNIT_TEST( test_InitBlock );
  CPPUNIT_TEST( test_KeySectionSize );
  CPPUNIT_TEST( test_KeySection_BodySize );
  CPPUNIT_TEST( test_DataSectionSize );
  CPPUNIT_TEST( test_DataSection_BodySize );
  CPPUNIT_TEST( test_GetKeyNeedLen );
  CPPUNIT_TEST( test_GetValueNeedLen );
  CPPUNIT_TEST( test_MakeKeyBuffer );
  CPPUNIT_TEST( test_MakeDeletedKeyBuffer );
  CPPUNIT_TEST( test_IsEnoughForData );
  CPPUNIT_TEST( test_GetKey );
  CPPUNIT_TEST( test_GetValue );
  CPPUNIT_TEST( test_MakeKeySectionBodyAndDataSectionBody );
  CPPUNIT_TEST( test_PackAndLoad );
  CPPUNIT_TEST( test_AddData );
  CPPUNIT_TEST( test_IndexFromBlock );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void test_constructor();
  void test_create();
  void test_InitBlock();
  void test_KeySectionSize();
  void test_DataSectionSize();
  void test_GetKeyNeedLen();
  void test_GetValueNeedLen();
  void test_MakeKeyBuffer();
  void test_MakeDeletedKeyBuffer();
  void test_KeySection_BodySize();
  void test_DataSection_BodySize();
  void test_IsEnoughForData();
  void test_GetKey();
  void test_GetValue();
  void test_MakeKeySectionBodyAndDataSectionBody();
  void test_PackAndLoad();
  void test_AddData();
  void test_IndexFromBlock();
};

#endif  // _BLOCK_STRUCTURE_TEST_H_
