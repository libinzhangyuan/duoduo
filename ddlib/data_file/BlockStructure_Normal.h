#ifndef DUO_BLOCK_STRUCTURE_NORMAL_H_
#define DUO_BLOCK_STRUCTURE_NORMAL_H_

#include <string>
#include "../typedef.h"
#include "BlockStructure.h"

namespace DuoDuo
{
    class BlockStructure_Normal : public BlockStructure
    {
    public:
        virtual bool IsEnoughForData(const std::string& block, const std::string& key, const std::string& value) const;
        virtual void AppendData(std::string& block, const std::string& key, const std::string& value);
    };
}

#endif
