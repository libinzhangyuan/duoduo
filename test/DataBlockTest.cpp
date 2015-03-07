// db_imp_test.cpp
#include <cstdlib>

#include <cppunit/config/SourcePrefix.h>
#include "DataBlockTest.h"

#include "test_def.h"

#include <block/BlockCreater.h>


#define private public
#define protected public
#include <block/DataBlock.h>
#include <block/BlockStructure.h>
#include <block/BlockStructure_Normal.h>
#include <block/BlockStructure_Big.h>
#include <block/BlockStructure_DataOnly.h>
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
        {
            DataBlock normal = BlockCreater::EmptyNormalBlock(64);
            DataBlock normal2(normal);
            CPPUNIT_ASSERT(&(normal2.GetBlockStructure().GetBlock()) == &normal2.m_Block);
            CPPUNIT_ASSERT(normal2.GetBlockStructure().BlockSize() == 64);
            CPPUNIT_ASSERT(normal2.Type() == BlockStructure::eBlockType_Normal);
        }
        {
            DataBlock big = BlockCreater::EmptyBigBlock(64);
            DataBlock big2(big);
            CPPUNIT_ASSERT(&(big2.GetBlockStructure().GetBlock()) == &big2.m_Block);
            CPPUNIT_ASSERT(big2.GetBlockStructure().BlockSize() == 64);
            CPPUNIT_ASSERT(big2.Type() == BlockStructure::eBlockType_Big);
        }
        {
            DataBlock dataOnly = BlockCreater::EmptyDataOnlyBlock(64);
            DataBlock dataOnly2(dataOnly);
            CPPUNIT_ASSERT(&(dataOnly2.GetBlockStructure().GetBlock()) == &dataOnly2.m_Block);
            CPPUNIT_ASSERT(dataOnly2.GetBlockStructure().BlockSize() == 64);
            CPPUNIT_ASSERT(dataOnly2.Type() == BlockStructure::eBlockType_DataOnly);
        }
    }
}

void DataBlockTest::test_create()
{
}

void DataBlockTest::test_assignment()
{
    {
        DataBlock normal = BlockCreater::EmptyNormalBlock(64);
        DataBlock normal2 = normal;
        CPPUNIT_ASSERT(&(normal2.GetBlockStructure().GetBlock()) == &normal2.m_Block);
        CPPUNIT_ASSERT(normal2.GetBlockStructure().BlockSize() == 64);
        CPPUNIT_ASSERT(normal2.Type() == BlockStructure::eBlockType_Normal);
    }
    {
        DataBlock big = BlockCreater::EmptyBigBlock(64);
        DataBlock big2 = big;
        CPPUNIT_ASSERT(&(big2.GetBlockStructure().GetBlock()) == &big2.m_Block);
        CPPUNIT_ASSERT(big2.GetBlockStructure().BlockSize() == 64);
        CPPUNIT_ASSERT(big2.Type() == BlockStructure::eBlockType_Big);

        DataBlock big3 = BlockCreater::EmptyBigBlock(64);
        big2 = big3;
    }
    {
        DataBlock dataOnly = BlockCreater::EmptyDataOnlyBlock(64);
        DataBlock dataOnly2 = dataOnly;
        CPPUNIT_ASSERT(&(dataOnly2.GetBlockStructure().GetBlock()) == &dataOnly2.m_Block);
        CPPUNIT_ASSERT(dataOnly2.GetBlockStructure().BlockSize() == 64);
        CPPUNIT_ASSERT(dataOnly2.Type() == BlockStructure::eBlockType_DataOnly);

        DataBlock dataOnly3 = BlockCreater::EmptyDataOnlyBlock(64);
        dataOnly2 = dataOnly3;
    }
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
            CPPUNIT_ASSERT(blocks.size() == 2);
            blocks[1] = BlockCreater::EmptyNormalBlock(64);
            CPPUNIT_ASSERT_THROW(DataBlock::GetData(blocks), Essential::AssertException);

            blocks[1] = BlockCreater::EmptyBigBlock(64);
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
            blocks[1] = another_blocks[1];
            CPPUNIT_ASSERT_THROW(DataBlock::GetData(blocks), Essential::AssertException);
        }
    }
}
