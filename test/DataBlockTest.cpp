// db_imp_test.cpp
#include <cstdlib>

#include <cppunit/config/SourcePrefix.h>
#include "DataBlockTest.h"

#include "test_def.h"

#include <block/DataBlock.h>
#include <block/BlockCreater.h>

#include <block/BlockStructure.h>
#include <block/BlockStructure_Normal.h>
#include <block/BlockStructure_Big.h>

#define private public
#define protected public
#undef private
#undef protected


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( DataBlockTest );

using namespace DuoDuo;
//using namespace Essential;

void DataBlockTest::setUp()
{
}


void DataBlockTest::tearDown()
{
}


void DataBlockTest::test_constructor()
{
    // copy constructor
    {
    }
}

void DataBlockTest::test_create()
{
}

void DataBlockTest::test_assignment()
{
}

void DataBlockTest::test_GetExtraBlockCount()
{
    // BigBlock
    {
        // in bound
        {
            DataBlock big = BlockCreater::EmptyBigBlock(64); // valueSize when keySize=10:  64 - 16 - 2 - 10 - 1 - 4 - 2 = 29
            CPPUNIT_ASSERT(BlockStructure_Big::StructCalc(64).MaxValueLen(10) == 29);
            big.AddData("1234567890", "12345678901234567890123456789");
            CPPUNIT_ASSERT(DataBlock::GetExtraBlockCount(big) == 0);
        }

        // out bound
        {
            DataBlock big = BlockCreater::EmptyBigBlock(64);
            big.AddData("1234567890", "123456789012345678901234567890");
            CPPUNIT_ASSERT(DataBlock::GetExtraBlockCount(big) == 1);
        }

        // more more data
            // maxValueLen of dataOnly(64) = 46; // 64 - 16 - 2;
        {
            // in bound
            {
                DataBlock big = BlockCreater::EmptyBigBlock(64);
                big.AddData("1234567890", "12345678901234567890123456789""1234567890123456789012345678901234567890123456");
                CPPUNIT_ASSERT(DataBlock::GetExtraBlockCount(big) == 1);
            }

            // out bound
            {
                DataBlock big = BlockCreater::EmptyBigBlock(64);
                big.AddData("1234567890", "12345678901234567890123456789""12345678901234567890123456789012345678901234567");
                CPPUNIT_ASSERT(DataBlock::GetExtraBlockCount(big) == 2);
            }
        }
    }

    // NormalBlock
    {
        DataBlock normal = BlockCreater::EmptyNormalBlock(64);
        CPPUNIT_ASSERT(DataBlock::GetExtraBlockCount(normal) == 0);
    }

    // DataOnlyBlock
    {
        DataBlock dataOnly = BlockCreater::EmptyDataOnlyBlock(64);
        CPPUNIT_ASSERT_THROW(DataBlock::GetExtraBlockCount(dataOnly), Essential::AssertException);
    }
}

void DataBlockTest::test_CreateDataAndGetData()
{
    // Big Data
    {
        // in bound
        {
            size_t blockSize = 64; // valueSize when keySize=10:  64 - 16 - 2 - 10 - 1 - 4 - 2 = 29
            std::string key = "1234567890";
            std::string value = "12345678901234567890123456789";
            const std::vector<DataBlock>& blocks = DataBlock::CreateBlockWithBigData(blockSize, key, value);
            const key_value_map_t& datas = DataBlock::GetData(blocks);
            CPPUNIT_ASSERT(datas.size() == 1);
            CPPUNIT_ASSERT(datas.find("1234567890") != datas.end());
            CPPUNIT_ASSERT(datas.find("1234567890")->second == "12345678901234567890123456789");
        }

        // out bound
        {
            size_t blockSize = 64; // valueSize when keySize=10:  64 - 16 - 2 - 10 - 1 - 4 - 2 = 29
            std::string key = "1234567890";
            std::string value = "123456789012345678901234567890";
            const std::vector<DataBlock>& blocks = DataBlock::CreateBlockWithBigData(blockSize, key, value);
            CPPUNIT_ASSERT(blocks.size() == 2);
            const key_value_map_t& datas = DataBlock::GetData(blocks);
            CPPUNIT_ASSERT(datas.size() == 1);
            CPPUNIT_ASSERT(datas.find("1234567890") != datas.end());
            CPPUNIT_ASSERT(datas.find("1234567890")->second == "123456789012345678901234567890");
        }

        // more more data
            // maxValueLen of dataOnly(64) = 46; // 64 - 16 - 2;
        {
            // in bound
            {
                size_t blockSize = 64; // valueSize when keySize=10:  64 - 16 - 2 - 10 - 1 - 4 - 2 = 29
                std::string key = "1234567890";
                std::string value = "12345678901234567890123456789""1234567890123456789012345678901234567890123456";
                const std::vector<DataBlock>& blocks = DataBlock::CreateBlockWithBigData(blockSize, key, value);
                CPPUNIT_ASSERT(blocks.size() == 2);
                const key_value_map_t& datas = DataBlock::GetData(blocks);
                CPPUNIT_ASSERT(datas.size() == 1);
                CPPUNIT_ASSERT(datas.find("1234567890")->second == value);
            }

            // out bound
            {
                size_t blockSize = 64; // valueSize when keySize=10:  64 - 16 - 2 - 10 - 1 - 4 - 2 = 29
                std::string key = "1234567890";
                std::string value = "12345678901234567890123456789""12345678901234567890123456789012345678901234567";
                const std::vector<DataBlock>& blocks = DataBlock::CreateBlockWithBigData(blockSize, key, value);
                CPPUNIT_ASSERT(blocks.size() == 3);
                const key_value_map_t& datas = DataBlock::GetData(blocks);
                CPPUNIT_ASSERT(datas.size() == 1);
                CPPUNIT_ASSERT(datas.find("1234567890")->second == value);
            }
        }

        // wrong type for latter block
        {
            size_t blockSize = 64; // valueSize when keySize=10:  64 - 16 - 2 - 10 - 1 - 4 - 2 = 29
            std::string key = "1234567890";
            std::string value = "12345678901234567890123456789""12345678901234567890123456789""1";
            std::vector<DataBlock> blocks = DataBlock::CreateBlockWithBigData(blockSize, key, value);
            blocks[2] = BlockCreater::EmptyNormalBlock(64);
            CPPUNIT_ASSERT_THROW(DataBlock::GetData(blocks), Essential::AssertException);

            blocks[2] = BlockCreater::EmptyBigBlock(64);
            CPPUNIT_ASSERT_THROW(DataBlock::GetData(blocks), Essential::AssertException);
        }

        // wrong dataLen for latter block
        {
            size_t blockSize = 64; // valueSize when keySize=10:  64 - 16 - 2 - 10 - 1 - 4 - 2 = 29
            std::string key = "1234567890";
            std::string value = "12345678901234567890123456789""12345678901234567890123456789""1";
            std::vector<DataBlock> blocks = DataBlock::CreateBlockWithBigData(blockSize, key, value);

            std::string another_value = "gadsfa;sdhjfhwefhauihfuhywefgagsdfkgasudgfgakdgfaisdgfagsdgfagsdfjagsdfdsfasdfasdfasdf";
            const std::vector<DataBlock>& another_blocks = DataBlock::CreateBlockWithBigData(blockSize, key, another_value);
            blocks[2] = another_blocks[2];
            CPPUNIT_ASSERT_THROW(DataBlock::GetData(blocks), Essential::AssertException);
        }
    }
}
