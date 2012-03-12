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
#ifndef INC_FUDGE_MEMORY_INTERNAL_H
#define INC_FUDGE_MEMORY_INTERNAL_H

#include "fudge/memory.h"
#include "fudge/status.h"

extern FudgeMemoryManager * s_memoryManager;

/* Initialise the memory manager. Usually only called from fudge.c. */
FudgeStatus FudgeMemory_init ( FudgeMemoryManager * manager );

/* Internal macros around memory manager - avoids function call overhead and
   hides the need to cast the return types */
#define FUDGEMEMORY_MALLOC( TYPE, SIZE ) ( TYPE ) s_memoryManager->allocate ( SIZE )
#define FUDGEMEMORY_REALLOC( TYPE, PTR, SIZE ) ( TYPE ) s_memoryManager->reallocate ( PTR, SIZE )
#define FUDGEMEMORY_FREE( PTR ) s_memoryManager->deallocate ( PTR )

#endif

