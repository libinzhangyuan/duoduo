// db_imp_test.cpp
#include <cstdlib>

#include <cppunit/config/SourcePrefix.h>
#include "DataFileTest.h"

#include "test_def.h"

#define private public
#define protected public
#include <DataFile.h>
#include <block/BlockStructure.h>
#include <block/BlockStructure_Normal.h>
#undef private
#undef protected


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( DataFileTest );

using namespace DuoDuo;
//using namespace Essential;

void DataFileTest::setUp()
{
}


void DataFileTest::tearDown()
{
}


void DataFileTest::test_constructor()
{
    {
        DataFile f("./test_tmp/", "name1.data", 2, 4096);
        CPPUNIT_ASSERT_EQUAL(f.GetFullPathFileName(), std::string("./test_tmp/name1.data"));
    }
}

void DataFileTest::test_create()
{
    // normal
    {
        {
            system("rm ./test_tmp/name1.data");
            DataFile* file = DataFile::Create("./test_tmp/", "name1", 64);
            CPPUNIT_ASSERT(file != NULL);
            CPPUNIT_ASSERT_EQUAL(file->GetFullPathFileName(), std::string("./test_tmp/name1.data"));
        }

        // folder not exist
        {
            system("rm ./test_tmp/duoduo_test_foler_not_exist/*");
            system("rmdir ./test_tmp/duoduo_test_foler_not_exist");
            DataFile* file = DataFile::Create("./test_tmp/duoduo_test_foler_not_exist/", "name1", 64);
            CPPUNIT_ASSERT(file != NULL);
            CPPUNIT_ASSERT_EQUAL(file->GetFullPathFileName(), std::string("./test_tmp/duoduo_test_foler_not_exist/name1.data"));
        }

        // file exist
        {
            system("echo '12345' >> ./test_tmp/name1.data");
            DataFile* file = DataFile::Create("./test_tmp/", "name1", 64);
            CPPUNIT_ASSERT(file != NULL);
            CPPUNIT_ASSERT_EQUAL(file->GetFullPathFileName(), std::string("./test_tmp/name1.data"));
        }
    }

    // block_size must be multiply of 16
    {
        system("rm ./test_tmp/name1.data");
        CPPUNIT_ASSERT_THROW(DataFile::Create("./test_tmp/", "name1", 63), Essential::AssertException);
        CPPUNIT_ASSERT_THROW(DataFile::Create("./test_tmp/", "name1", 0), Essential::AssertException);
    }

    // folder permission not allowed
    {
        {
            DataFile* file = DataFile::Create("/etc/", "name1", 64);
            CPPUNIT_ASSERT(file == NULL);
        }
        {
            DataFile* file = DataFile::Create("/etc/haha/", "name1", 64);
            CPPUNIT_ASSERT(file == NULL);
        }
    }

    // file permission not allowed
    {
        DataFile* file = DataFile::Create("/etc/", "hosts", 64);
        CPPUNIT_ASSERT(file == NULL);
    }
}

void DataFileTest::test_AddData(void)
{
    // normal
    {
        system("rm ./test_tmp/AddData.data");
        DataFile* file = DataFile::Create("./test_tmp/", "AddData", 64);
        CPPUNIT_ASSERT(file != NULL);
        file->AddData("123456", "dw");
        file->AddData("7890", "2323");
        file->AddData("123456", "new");

        CPPUNIT_ASSERT(file->m_CachedKeyValues.m_CachedKeyValues["123456"] == std::string("new"));
        CPPUNIT_ASSERT(file->m_CachedKeyValues.m_CachedKeyValues["7890"] == std::string("2323"));
    }
}

void DataFileTest::test_HasSmallData()
{
    // key too large for a empty normalBlock
    {
        DuoDuo::DataFile::CachedKeyValueContainer container(128);
        CPPUNIT_ASSERT(container.HasSmallData() == false);
        container.AddData("a23456789012345678", "gdf");
        container.AddData("b23456789012345678", "gdf");
        CPPUNIT_ASSERT(container.HasSmallData() == false);
        container.AddData("12345678901234567", "gasdf");
        CPPUNIT_ASSERT(container.HasSmallData() == true);
    }

    // value too large for a empty normalBlock
    {
        DuoDuo::DataFile::CachedKeyValueContainer container(80); // data section: 53 = (80 * 11 / 16) - 2
        container.AddData("2", "2v3456789012345678901234567890""123456789012345678901");
        container.AddData("53", "2v3456789012345678901234567890""123456789012345678901");
        CPPUNIT_ASSERT(container.HasSmallData() == false);
        container.AddData("3", "2v3456789012345678901234567890""12345678901234567890");
        CPPUNIT_ASSERT(container.HasSmallData() == true);
    }

}

void DataFileTest::test_HasBigData()
{
    // key too large for a empty normalBlock
    {
        DuoDuo::DataFile::CachedKeyValueContainer container(128);
        CPPUNIT_ASSERT(container.HasBigData() == false);
        container.AddData("a2345678901234567", "gdf");
        container.AddData("b2345678901234567", "gdf");
        CPPUNIT_ASSERT(container.HasBigData() == false);
        container.AddData("123456789012345678", "gasdf");
        CPPUNIT_ASSERT(container.HasBigData() == true);
    }

    // value too large for a empty normalBlock
    {
        DuoDuo::DataFile::CachedKeyValueContainer container(80); // data section: 53 = (80 * 11 / 16) - 2
        container.AddData("2", "2v3456789012345678901234567890""12345678901234567890");
        container.AddData("53", "2v3456789012345678901234567890""1234567890123456789");
        CPPUNIT_ASSERT(container.HasBigData() == false);
        container.AddData("3", "2v3456789012345678901234567890""123456789012345678901");
        CPPUNIT_ASSERT(container.HasBigData() == true);
    }


}
