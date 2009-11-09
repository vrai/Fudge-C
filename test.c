#include "types.h"

int main (int argc, char* argv[])
{
    FudgeTypeDictionaryRef dictionary = FudgeTypeDictionary_create ( );

    if ( FudgeTypeDictionary_dumpToFile ( dictionary, stdout ) )
        fprintf ( stderr, "\nEpic fail\n" );
    fprintf ( stdout, "\n" );

    FudgeTypeDictionary_destroy ( dictionary );

    return 0;
}

