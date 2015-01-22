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

    private:
        size_t m_KeySectionSize;
        size_t m_DataSectionSize;
        friend class BlockStructure_NormalTest;
        friend struct CppUnit::Asserter;
        //friend void CppUnit::Asserter::failIf(bool, const Message&, const SourceLine&);
    };
}

#endif
