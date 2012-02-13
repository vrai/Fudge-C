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
#include "memory_internal.h"
#include <stdlib.h>
#include <stdio.h>

FudgeMemoryManager * s_memoryManager = 0;

FudgeMemoryManager FudgeMemory_default =
{
    malloc,     /* Allocate */
    free        /* Deallocate */
};

FudgeStatus FudgeMemory_init ( FudgeMemoryManager * manager )
{
    if ( ! manager )
        return FUDGE_NULL_POINTER;

    if ( s_memoryManager && s_memoryManager != manager )
        return FUDGE_CHANGED_MEMORY_MANAGER;

    s_memoryManager = manager;
    return FUDGE_OK;
}

FudgeMemoryManager * FudgeMemory_defaultManager ( )
{
    return &FudgeMemory_default;
}

void * FudgeMemory_malloc ( size_t size )
{
    return s_memoryManager->allocate ( size );
}

void FudgeMemory_free ( void * ptr )
{
    s_memoryManager->deallocate ( ptr );
}

