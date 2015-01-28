#ifndef DUO_TYPEDEF_H_
#define DUO_TYPEDEF_H_

#include <unistd.h>
#include <map>

#include <binary_stream/binary_buf.hpp>


namespace DuoDuo
{
    typedef off_t f_offset;
    typedef Essential::_binary_buf block_t;

    typedef std::map<std::string /*key*/, std::string /*value*/> key_value_map_t;
}

#endif
