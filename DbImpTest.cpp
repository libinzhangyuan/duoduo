// db_imp_test.cpp

#include <cppunit/config/SourcePrefix.h>
#include "DbImp.h"
#include "DbImpTest.h"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( DBImpTest );

using namespace DuoDuo;

void 
DBImpTest::setUp()
{
}


void 
DBImpTest::tearDown()
{
}


void 
DBImpTest::testConstructor()
{
    // folder should support with / or not.
    {

        DBImp db1("./test_tmp/", "name1");
        CPPUNIT_ASSERT_EQUAL(db1.get_folder(), std::string("./test_tmp/"));
        DBImp db2("./test_tmp", "name2");
        CPPUNIT_ASSERT_EQUAL(db2.get_folder(), std::string("./test_tmp/"));
    }

}
