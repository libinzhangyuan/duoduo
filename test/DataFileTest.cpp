// db_imp_test.cpp

#include <cppunit/config/SourcePrefix.h>
#include <data_file/DataFile.h>
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
        DataFile f("./test_tmp/", "name1.data", 2);
        CPPUNIT_ASSERT_EQUAL(f.get_full_path_file_name(), std::string("./test_tmp/name1.data"));
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
            CPPUNIT_ASSERT_EQUAL(file->get_full_path_file_name(), std::string("./test_tmp/name1.data"));
        }

        // folder not exist
        {
            system("rm ./test_tmp/duoduo_test_foler_not_exist/*");
            system("rmdir ./test_tmp/duoduo_test_foler_not_exist");
            DataFile* file = DataFile::create("./test_tmp/duoduo_test_foler_not_exist/", "name1");
            CPPUNIT_ASSERT(file != NULL);
            CPPUNIT_ASSERT_EQUAL(file->get_full_path_file_name(), std::string("./test_tmp/duoduo_test_foler_not_exist/name1.data"));
        }

        // file exist
        {
            system("echo '12345' >> ./test_tmp/name1.data");
            DataFile* file = DataFile::create("./test_tmp/", "name1");
            CPPUNIT_ASSERT(file != NULL);
            CPPUNIT_ASSERT_EQUAL(file->get_full_path_file_name(), std::string("./test_tmp/name1.data"));
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

void DataFileTest::test_add_data(void)
{
    // normal
    {
        system("rm ./test_tmp/add_data.data");
        DataFile* file = DataFile::create("./test_tmp/", "add_data");
        CPPUNIT_ASSERT(file != NULL);
        CPPUNIT_ASSERT(file->add_data(std::string("123456")) == 0);
        CPPUNIT_ASSERT(file->add_data(std::string("7890")) == 6);
        CPPUNIT_ASSERT(file->get_data(0, 6) == std::string("123456"));
        CPPUNIT_ASSERT(file->get_data(6, 4) == std::string("7890"));

    }
}

void DataFileTest::test_get_data(void)
{
    // normal
    {
        system("rm ./test_tmp/add_data.data");
        DataFile* file = DataFile::create("./test_tmp/", "add_data");
        CPPUNIT_ASSERT(file != NULL);

        CPPUNIT_ASSERT(file->add_data(std::string("123456")) == 0);
        CPPUNIT_ASSERT(file->get_data(0, 6) == std::string("123456"));

        CPPUNIT_ASSERT(file->add_data(std::string("7890")) == 6);
        CPPUNIT_ASSERT(file->get_data(0, 6) == std::string("123456"));
        CPPUNIT_ASSERT(file->get_data(6, 4) == std::string("7890"));
    }
}
