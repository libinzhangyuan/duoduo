// db_imp_test.cpp
#include <cstdlib>

#include <cppunit/config/SourcePrefix.h>
#include "DataFileTest.h"

#include "test_def.h"
#include "test_util.h"

#include <block/BlockStructure_Big.h>
#include <block/BlockStructure_DataOnly.h>

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

void DataFileTest::test_PopBigBlock()
{
    {
        DuoDuo::DataFile::CachedKeyValueContainer container(128); // BigBlock: valueSize when keySize=18:  128 - 16 - 2 - 18 - 1 - 4 - 2 = 85
                                                                  // NormalBlock: keySection  22   // (128 * 5 / 16) - 16 - 2
                                                                  // NormalBlock: DataSection 86   // (128 * 11 / 16) - 2

        // empty
        {
            CPPUNIT_ASSERT(container.PopBigBlock().size() == 0);
        }

        // have small data only
        {
            container.AddData("a2345678901234567", "gdf");
            container.AddData("b2345678901234567", "gdf");
            CPPUNIT_ASSERT(container.PopBigBlock().size() == 0);
        }

        // has one big data
        {
            CPPUNIT_ASSERT(test_str("123", 18).size() == 18);
            container.AddData(test_str("123", 18), "gasdf");
            const std::vector<DataBlock>& blocks = container.PopBigBlock();
            CPPUNIT_ASSERT(blocks.size() == 1);
            CPPUNIT_ASSERT(blocks[0].Type() == BlockStructure::eBlockType_Big);

            // todo get data by index and check the data.

            BlockStructure_Big* pStruct = dynamic_cast<BlockStructure_Big*>(blocks[0].m_pBlockStructure);
            CPPUNIT_ASSERT(pStruct->GetKey() == test_str("123", 18));
            CPPUNIT_ASSERT(pStruct->GetValueStoredInBlock() == "gasdf");
            CPPUNIT_ASSERT(pStruct->GetValueTotalLen() == 5);

            CPPUNIT_ASSERT(container.PopBigBlock().size() == 0);
        }

        // has one big data that need dataOnlyBlock for extra value
        {
            // in bound
            {
                container.AddData(test_str("ia", 18), test_str("value85", 85));
                const std::vector<DataBlock>& blocks = container.PopBigBlock();
                CPPUNIT_ASSERT(blocks.size() == 1);
                BlockStructure_Big* pStruct0 = dynamic_cast<BlockStructure_Big*>(blocks[0].m_pBlockStructure);
                CPPUNIT_ASSERT(pStruct0->GetKey() == test_str("ia", 18));
                CPPUNIT_ASSERT(pStruct0->GetValueStoredInBlock() == test_str("value85", 85));
                CPPUNIT_ASSERT(pStruct0->GetValueTotalLen() == 85);

                CPPUNIT_ASSERT(container.PopBigBlock().size() == 0);
            }

            // out bound
            {
                container.AddData(test_str("ia", 18), test_str("value86", 86));
                const std::vector<DataBlock>& blocks = container.PopBigBlock();
                CPPUNIT_ASSERT(blocks.size() == 2);
                CPPUNIT_ASSERT(blocks[0].Type() == BlockStructure::eBlockType_Big);
                CPPUNIT_ASSERT(blocks[1].Type() == BlockStructure::eBlockType_DataOnly);
                BlockStructure_Big* pStruct0 = dynamic_cast<BlockStructure_Big*>(blocks[0].m_pBlockStructure);
                CPPUNIT_ASSERT(pStruct0->GetKey() == test_str("ia", 18));
                CPPUNIT_ASSERT(pStruct0->GetValueStoredInBlock() == test_str("value86", 85));
                CPPUNIT_ASSERT(pStruct0->GetValueTotalLen() == 86);

                const BlockStructure_DataOnly* pStruct1 = dynamic_cast<const BlockStructure_DataOnly*>(blocks[1].GetBlockStructurePtr());
                CPPUNIT_ASSERT(pStruct1->GetValueStoredInBlock() == "6");

                CPPUNIT_ASSERT(container.PopBigBlock().size() == 0);
            }

            // more more dataOnlyBlock
            {
                container.AddData(test_str("ia", 18), test_str("value86", 86));
                container.AddData(test_str("ib", 18), test_str("value86", 86));
                container.AddData(test_str("ic", 18), test_str("value86", 86));
                const std::vector<DataBlock>& blocks0 = container.PopBigBlock();
                CPPUNIT_ASSERT(blocks0.size() == 2);
                const std::vector<DataBlock>& blocks1 = container.PopBigBlock();
                CPPUNIT_ASSERT(blocks1.size() == 2);
                const std::vector<DataBlock>& blocks2 = container.PopBigBlock();
                CPPUNIT_ASSERT(blocks2.size() == 2);
                CPPUNIT_ASSERT(container.PopBigBlock().size() == 0);
            }

        }

        // has more big data
    }
}

void DataFileTest::test_PopSmallBlock()
{
    {
        DuoDuo::DataFile::CachedKeyValueContainer container(128); // BigBlock: valueSize when keySize=18:  128 - 16 - 2 - 18 - 1 - 4 - 2 = 85
        // NormalBlock: keySection  22   // (128 * 5 / 16) - 16 - 2
        // NormalBlock: DataSection 86   // (128 * 11 / 16) - 2

        // empty
        {
            const DataBlock& block = container.PopSmallBlock();
            CPPUNIT_ASSERT(block.IsEmpty() == true);
            key_value_map_t datas = DataBlock::GetData(block);
            CPPUNIT_ASSERT(datas.size() == 0);
        }

        // have big data only
        {
            container.AddData(test_str("123", 18), "gasdf");
            const DataBlock& block = container.PopSmallBlock();
            CPPUNIT_ASSERT(block.IsEmpty() == true);
            key_value_map_t datas = DataBlock::GetData(block);
            CPPUNIT_ASSERT(datas.size() == 0);
        }

        // has one small data
        {
            container.AddData("123", "gasdf");
            const DataBlock& block = container.PopSmallBlock();
            CPPUNIT_ASSERT(block.Type() == BlockStructure::eBlockType_Normal);
            CPPUNIT_ASSERT(block.IsEmpty() == false);
            key_value_map_t datas = DataBlock::GetData(block);
            CPPUNIT_ASSERT(datas.size() == 1);
            CPPUNIT_ASSERT(datas["123"] == "gasdf");

            const DataBlock& block2 = container.PopSmallBlock();
            CPPUNIT_ASSERT(block2.IsEmpty() == true);
        }

        // has more small data
        {
            // in bound
            {
                container.AddData("1", "1");   // key 6, value 4
                container.AddData("22", "22"); // key 7, value 5
                container.AddData("4444", test_str("a74", 74)); // key 9, value 74
                const DataBlock& block = container.PopSmallBlock();
                CPPUNIT_ASSERT(block.Type() == BlockStructure::eBlockType_Normal);
                key_value_map_t datas = DataBlock::GetData(block);
                CPPUNIT_ASSERT(datas["1"] == "1");
                CPPUNIT_ASSERT(datas["22"] == "22");
                CPPUNIT_ASSERT(datas["4444"] == test_str("a74", 74));

                const DataBlock& block2 = container.PopSmallBlock();
                CPPUNIT_ASSERT(block2.IsEmpty() == true);
            }

            // out bound  key
            {
                container.AddData("1", "1");   // key 6, value 4
                container.AddData("22", "22"); // key 7, value 5
                container.AddData("44441", test_str("a74", 74)); // key 9, value 74
                const DataBlock& block0 = container.PopSmallBlock();
                CPPUNIT_ASSERT(block0.Type() == BlockStructure::eBlockType_Normal);
                const DataBlock& block1 = container.PopSmallBlock();
                CPPUNIT_ASSERT(block1.Type() == BlockStructure::eBlockType_Normal);
                key_value_map_t datas0 = DataBlock::GetData(block0);
                key_value_map_t datas1 = DataBlock::GetData(block1);
                CPPUNIT_ASSERT(datas1.size() + datas0.size() == 3);
                datas1.insert(datas0.begin(), datas0.end());
                CPPUNIT_ASSERT(datas1.size() == 3);
                CPPUNIT_ASSERT(datas1["1"] == "1");
                CPPUNIT_ASSERT(datas1["22"] == "22");
                CPPUNIT_ASSERT(datas1["44441"] == test_str("a74", 74));

                const DataBlock& block2 = container.PopSmallBlock();
                CPPUNIT_ASSERT(block2.IsEmpty() == true);
            }

            // out bound   value
            {
                container.AddData("1", "1");   // key 6, value 4
                container.AddData("22", "223"); // key 7, value 5
                container.AddData("4444", test_str("a74", 74)); // key 9, value 74
                const DataBlock& block0 = container.PopSmallBlock();
                CPPUNIT_ASSERT(block0.Type() == BlockStructure::eBlockType_Normal);
                const DataBlock& block1 = container.PopSmallBlock();
                CPPUNIT_ASSERT(block1.Type() == BlockStructure::eBlockType_Normal);
                key_value_map_t datas0 = DataBlock::GetData(block0);
                key_value_map_t datas1 = DataBlock::GetData(block1);
                CPPUNIT_ASSERT(datas1.size() + datas0.size() == 3);
                datas1.insert(datas0.begin(), datas0.end());
                CPPUNIT_ASSERT(datas1.size() == 3);
                CPPUNIT_ASSERT(datas1["1"] == "1");
                CPPUNIT_ASSERT(datas1["22"] == "223");
                CPPUNIT_ASSERT(datas1["4444"] == test_str("a74", 74));


                const DataBlock& block2 = container.PopSmallBlock();
                CPPUNIT_ASSERT(block2.IsEmpty() == true);
            }

        }
    }
}
