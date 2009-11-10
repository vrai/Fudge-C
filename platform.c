#include "platform.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

char * strdup ( const char * string )
{
    unsigned long length;
    char * copy;

    assert ( string );

    length = strlen ( string );
    copy = ( char * ) malloc ( length + 1 );
    if ( copy )
        memcpy ( copy, string, length + 1 );
    return copy;
}

