/**
 * Copyright (C) 2010 - 2010, Vrai Stacey.
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
#ifndef INC_FUDGE_STRINGPOOL_H
#define INC_FUDGE_STRINGPOOL_H

#include "fudge/string.h"

#ifdef __cplusplus
    extern "C" {
#endif

typedef struct FudgeStringPoolImpl * FudgeStringPool;

FUDGEAPI FudgeStatus FudgeStringPool_create ( FudgeStringPool * pool );
FUDGEAPI FudgeStatus FudgeStringPool_retain ( FudgeStringPool pool );
FUDGEAPI FudgeStatus FudgeStringPool_release ( FudgeStringPool pool );

/* Adds the string to the pool and hands over responsibility to release the
   string's reference. When the pool is cleared FudgeString_release will be
   called ONCE on the string. The calling code must NOT release the string
   without first retaining another reference to it.

   The same string can be added to a single pool multiple times (or multiple
   times to multiple pools). Each acquisition will result in an additional
   release at cleanup time, so the string must have been retained at least
   as many times (including the automatic retain that occurs on string
   creation). */
FUDGEAPI FudgeStatus FudgeStringPool_acquire ( FudgeStringPool pool, FudgeString string );

/* Calls release once per string acquired and clears the pool. The pool
   structure itself is not destroyed and can be reused. */
FUDGEAPI FudgeStatus FudgeStringPool_clear ( FudgeStringPool pool );

/* Helper functions that mimic the FudgeString constructors. They create a new
   FudgeString instance and the pool acquires. The new string is returned and
   can be used as normal. It should NOT be released without an additional
   retain call; the pool will release the initial reference when cleared.
   If the string creation fails, NULL will be returned and the status varible
   updated. */
FUDGEAPI FudgeString FudgeStringPool_createStringFromASCII ( FudgeStringPool pool, FudgeStatus * status, const char * chars, size_t numchars );
FUDGEAPI FudgeString FudgeStringPool_createStringFromASCIIZ ( FudgeStringPool pool, FudgeStatus * status, const char * chars );

#ifdef __cplusplus
    }
#endif

#endif


