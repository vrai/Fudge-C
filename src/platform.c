#include "fudge/platform.h"
#include <assert.h>
#ifdef FUDGE_HAVE_MATH_H
#include <math.h>
#endif /* ifdef FUDGE_HAVE_MATH_H */
#ifdef FUDGE_HAVE_FLOAT_H
#include <float.h>
#endif /* ifdef FUDGE_HAVE_FLOAT_H */

#ifdef _BIG_ENDIAN

float ntohf ( float value )
{
    return value;
}

float htonf ( float value )
{
    return value;
}

double ntohd ( double value )
{
    return value;
}

double htond ( double value )
{
    return value;
}

int64_t ntohi64 ( int64_t value )
{
    return value;
}

int64_t htoni64 ( int64_t value )
{
    return value;
}

#else

#define JAVA_FLOAT_NAN  ( int32_t ) 0x7fc00000
#define JAVA_DOUBLE_NAN ( int64_t ) 0x7ff8000000000000ll

#if !defined(FUDGE_HAS_ISNAN) && !defined(isnan)
#define isnan _isnan
#endif /* if !defined(FUDGE_HAS_ISNAN) && !defined(isnan) */

/* inline suppressed for MSVC compatibility. Do we need this? Won't modern compilers spot this? */
/* inline */ float convertBytesToFloat ( int32_t bytes )
{
#ifdef NAN
    if ( bytes == JAVA_FLOAT_NAN )
        return NAN;
#endif
    return *( ( float * ) ( &bytes ) );
}

float ntohf ( float value )
{
    int32_t bytes;
    bytes = ntohl ( *( ( int32_t * ) ( &value ) ) );
    return convertBytesToFloat ( bytes );
}

float htonf ( float value )
{
    int32_t bytes;
    if ( value != value || isnan ( value ) )
        bytes = JAVA_FLOAT_NAN;
    else
        bytes = *( ( int32_t * ) ( &value ) );
    bytes = htonl ( bytes );
    return *( ( float * ) ( &bytes ) );
}

double ntohd ( double value )
{
    int64_t bytes;
    bytes = ntohi64 ( *( ( int64_t * ) ( &value ) ) );
    return *( ( double * ) ( &bytes ) );
}

double htond ( double value )
{
    int64_t bytes;
    if ( value != value || isnan ( value ) )
        bytes = JAVA_DOUBLE_NAN;
    else
        bytes = *( ( int64_t * ) ( &value ) );
    bytes = htoni64 ( bytes );
    return *( ( double * ) ( &bytes ) );
}

int64_t ntohi64 ( int64_t value )
{
    return ( ( value >> 56 ) & ( int64_t ) 0x00000000000000ffll ) |
           ( ( value >> 40 ) & ( int64_t ) 0x000000000000ff00ll ) |
           ( ( value >> 24 ) & ( int64_t ) 0x0000000000ff0000ll ) |
           ( ( value >>  8 ) & ( int64_t ) 0x00000000ff000000ll ) |
           ( ( value <<  8 ) & ( int64_t ) 0x000000ff00000000ll ) |
           ( ( value << 24 ) & ( int64_t ) 0x0000ff0000000000ll ) |
           ( ( value << 40 ) & ( int64_t ) 0x00ff000000000000ll ) |
           ( ( value << 56 ) & ( int64_t ) 0xff00000000000000ll );
}

int64_t htoni64 ( int64_t value )
{
    return ntohi64 ( value );
}

#endif

