#ifndef DUO_TYPEDEF_H_
#define DUO_TYPEDEF_H_

#include <unistd.h>
#include <map>

#include <binary_stream/binary_buf.hpp>


namespace DuoDuo
{
    typedef off_t f_offset;
    typedef Essential::_binary_buf block_t;

    // The index of block from beginning of the file. The index of first block in file will be 0.
    typedef uint64_t block_index_t;

    // position in one block
    typedef uint16_t pos_in_block_t;

    struct index_t
    {
        block_index_t block_index;
        pos_in_block_t pos_in_block;
    };

    typedef std::map<std::string /*key*/, std::string /*value*/> key_value_map_t;
}

#endif
