#ifndef INC_FUDGE_PLATFORM_H
#define INC_FUDGE_PLATFORM_H

#if HAVE_CONFIG_H
#   include "fudge/config.h"
#endif

/* Include the availale system headers. */
#ifdef FUDGE_HAVE_SYS_TYPES_H
#   include <sys/types.h>
#endif
#ifdef FUDGE_HAVE_INTTYPES_H
#   include <inttypes.h>
#endif
#ifdef FUDGE_HAVE_STDINT_H
#   include <stdint.h>
#endif
#ifdef FUDGE_HAVE_STDLIB_H
#   include <stdlib.h>
#endif 
#ifdef FUDGE_HAVE_STRING_H
#   include <string.h>
#endif
#include <arpa/inet.h>

#ifdef __cplusplus
    extern "C" {
#endif

/* Convert floats (32 bit floating point values) between network and host byte
   ordering. These are no-ops on big-endian architectures. */
extern float ntohf ( float value );
extern float htonf ( float value );

/* As above, but for doubles (64 bit floating point values). */
extern double ntohd ( double value );
extern double htond ( double value );

/* As above, but for 64 bit integers. */
extern int64_t ntohi64 ( int64_t value );
extern int64_t htoni64 ( int64_t value );

#ifndef FUDGE_HAS_STRDUP

/* True C99 implementations don't have an strdup implementation, so provide one here
   if required. */
extern char * strdup ( const char * string );

#endif

#ifdef __cplusplus
    }
#endif

#endif

