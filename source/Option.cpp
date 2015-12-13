#include <cstring>
#include "Option.h"

bool OptionExists( int argc, char* argv[], const char* pcstrOption )
{
    for ( int i = 0; i < argc; ++i )
    {
        if ( std::strcmp(argv[i], pcstrOption) == 0 )
        {
            return true;
        }
    }

    return false;
}

char* OptionValue( int argc, char* argv[], const char* pcstrOption )
{
    for ( int i = 0; i < argc; ++i )
    {
        if ( std::strcmp(argv[i], pcstrOption) == 0 )
        {
            if ( i+1 < argc )
            {
                return argv[i+1];
            }
        }
    }

    return NULL;
}

