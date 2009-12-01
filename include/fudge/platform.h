/**
 * Copyright (C) 2009 - 2009, Vrai Stacey.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 *     
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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

