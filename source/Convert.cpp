#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Convert.h"

// static
bool Convert::IsDouble( const char* pStr )
{
    char* pEnd;
    std::strtod( pStr, &pEnd );

    if ( pEnd != pStr )
        while ( std::isspace(*pEnd) )
            ++pEnd;

    return pStr != pEnd && *pEnd == '\0';
}

// static
bool Convert::IsDecimal( const char* pStr )
{
    char* pEnd;
    std::strtol( pStr, &pEnd, 10 );

    if ( pEnd != pStr )
        while ( std::isspace(*pEnd) )
            ++pEnd;

    return pStr != pEnd && *pEnd == '\0';
}


// static
double Convert::ToDouble( const char* pStr )
{
    char* pEnd;
    double result = std::strtod( pStr, &pEnd );

    assert( pStr != pEnd );
    return result;
}

// static
long int Convert::ToDecimal( const char* pStr )
{
    char* pEnd;
    long int result = std::strtol( pStr, &pEnd, 10 );

    assert( pStr != pEnd );
    return result;
}
