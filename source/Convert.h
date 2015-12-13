#ifndef CONVERT_H_INCLUDED
#define CONVERT_H_INCLUDED

class Convert
{
public:
    static bool IsDouble( const char* pStr );
    static bool IsDecimal( const char* pStr );
    static double ToDouble( const char* pStr );
    static long int ToDecimal( const char* pStr );
};

/*
  Ref.: http://stackoverflow.com/questions/332852/check-variable-type-in-c
*/

#endif // CONVERT_H_INCLUDED
