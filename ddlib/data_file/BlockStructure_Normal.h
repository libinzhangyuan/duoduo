#ifndef DUO_BLOCK_STRUCTURE_NORMAL_H_
#define DUO_BLOCK_STRUCTURE_NORMAL_H_

#include <string>
#include "../typedef.h"
#include "BlockStructure.h"

namespace CppUnit {
    struct Asserter;
};

namespace DuoDuo
{
    class BlockStructure_Normal : public BlockStructure
    {
    public:

        BlockStructure_Normal(std::string& block);
        ~BlockStructure_Normal(void) {}

        virtual bool IsEnoughForData(const std::string& block, const std::string& key, const std::string& value) const;
        virtual void AppendData(std::string& block, const std::string& key, const std::string& value);

    private:
        size_t KeySectionSize(void) const;
        size_t DataSectionSize(void) const;

        static size_t GetKeyNeedLen(const std::string& key);
        static size_t GetValueNeedLen(const std::string& value);

    private:
        size_t m_KeySectionSize;
        size_t m_DataSectionSize;
        friend class BlockStructure_NormalTest;
    };
}

#endif
