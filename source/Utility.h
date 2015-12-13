#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include "Types.h"

inline void WriteFourCC( uint32* out, uint8 v0, uint8 v1, uint8 v2, uint8 v3, bool swap_endian = false )
{
    union {
        uint32 native;
        uint8 byte[4];
    } v;

    v.byte[0] = v0;
    v.byte[1] = v1;
    v.byte[2] = v2;
    v.byte[3] = v3;

    if ( swap_endian )
        *out = (v.byte[0] << 24) | (v.byte[1] << 16) | (v.byte[2] <<  8) | (v.byte[3] << 0);
    else
        *out = v.native;
}

inline void WriteValueU32( uint32* out, uint32 in, bool swap_endian = false )
{
    union {
        uint32 native;
        uint8 byte[4];
    } v;

    v.native = in;

    if ( swap_endian )
        *out = (v.byte[0] << 24) | (v.byte[1] << 16) | (v.byte[2] <<  8) | (v.byte[3] << 0);
    else
        *out = v.native;
}

inline unsigned int RoundUp( unsigned int value, unsigned int align )
{
    return (value+(align-1)) & (~(align-1));
}


#endif // UTILITY_H_INCLUDED
