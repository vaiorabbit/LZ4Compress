#ifndef OPTION_H_INCLUDED
#define OPTION_H_INCLUDED

bool OptionExists( int argc, char* argv[], const char* pcstrOption );

char* OptionValue( int argc, char* argv[], const char* pcstrOption );

#endif // OPTION_H_INCLUDED
