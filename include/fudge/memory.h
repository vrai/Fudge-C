/**
 * Copyright (C) 2012 - 2012, Vrai Stacey.
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
#ifndef INC_FUDGE_MEMORY_H
#define INC_FUDGE_MEMORY_H

#include "fudge/platform.h"

#ifdef __cplusplus
    extern "C" {
#endif

/* The Fudge-C memory manager structure. Holds pointers to the allocator
   and deallocator functions that are used by the library. If none is
   specified by Fudge_init the default implementation will be used (see
   below). Any user implementation must meet the following assumptions:

   1. The allocator will return NULL on failure.
   2. The deallocator will ignore NULL pointers (i.e. attempting to free
      NULL is a noop).
   3. The manager is safe to call from multiple threads concurrently; all
      thread safety is handled internally.
   4. Any construction or cleanup of the manager itself must be handled
      outside of the Fudge-C library.
*/
typedef struct
{
    void * ( *allocate ) ( size_t );
    void ( *deallocate ) ( void * );

} FudgeMemoryManager;

/* Returns a pointer to the default memory manager (the one that will be
   used if none is specified using Fudge_init). The default manager simply
   calls the system's malloc and free implementations. */
FUDGEAPI FudgeMemoryManager * FudgeMemory_defaultManager ( );

/* Access to the current memory managers allocate/deallocate
   functionality */
FUDGEAPI void * FudgeMemory_malloc ( size_t size );
FUDGEAPI void FudgeMemory_free ( void * ptr );

#ifdef __cplusplus
    }
#endif

#endif

