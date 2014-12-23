// db_imp_test.cpp

#include <cppunit/config/SourcePrefix.h>
#include "DataFile.h"
#include "DataFileTest.h"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( DataFileTest );

using namespace DuoDuo;

void DataFileTest::setUp()
{
}


void DataFileTest::tearDown()
{
}


void DataFileTest::test_constructor()
{
    {
        DataFile f("./test_tmp/", "name1", 2);
        CPPUNIT_ASSERT_EQUAL(f.get_full_path_file_name(), std::string("./test_tmp/name1"));
    }
}

void DataFileTest::test_create()
{
    // normal
    {
        {
            system("rm ./test_tmp/name1.data");
            DataFile* file = DataFile::create("./test_tmp/", "name1");
            CPPUNIT_ASSERT(file != NULL);
            CPPUNIT_ASSERT_EQUAL(file->get_full_path_file_name(), std::string("./test_tmp/name1"));
        }

        // folder not exist
        {
            system("rm ./test_tmp/duoduo_test_foler_not_exist/*");
            system("rmdir ./test_tmp/duoduo_test_foler_not_exist");
            DataFile* file = DataFile::create("./test_tmp/duoduo_test_foler_not_exist/", "name1");
            CPPUNIT_ASSERT(file != NULL);
            CPPUNIT_ASSERT_EQUAL(file->get_full_path_file_name(), std::string("./test_tmp/duoduo_test_foler_not_exist/name1"));
        }

        // file exist
        {
            system("echo '12345' >> ./test_tmp/name1.data");
            DataFile* file = DataFile::create("./test_tmp/", "name1");
            CPPUNIT_ASSERT(file != NULL);
            CPPUNIT_ASSERT_EQUAL(file->get_full_path_file_name(), std::string("./test_tmp/name1"));
        }
    }

    // folder permission not allowed
    {
        {
            DataFile* file = DataFile::create("/etc/", "name1");
            CPPUNIT_ASSERT(file == NULL);
        }
        {
            DataFile* file = DataFile::create("/etc/haha/", "name1");
            CPPUNIT_ASSERT(file == NULL);
        }
    }

    // file permission not allowed
    {
        DataFile* file = DataFile::create("/etc/", "hosts");
        CPPUNIT_ASSERT(file == NULL);
    }
}
