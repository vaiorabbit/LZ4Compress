#ifndef FORMAT_H_INCLUDED
#define FORMAT_H_INCLUDED

#include "Types.h"

struct BinaryHeader
{
    uint32 FourCC;     // 'L','Z','4','A'
    uint32 FileSize;   // Original (uncompressed) size of file
    uint32 BlockSize;  // Block Size
    uint32 BlockCount; // # of blocks
};

#endif // FORMAT_H_INCLUDED
