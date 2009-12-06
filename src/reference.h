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
#ifndef INC_FUDGE_REFERENCE_H
#define INC_FUDGE_REFERENCE_H

#include "fudge/status.h"

#ifdef __cplusplus
    extern "C" {
#endif

typedef struct FudgeRefCountImpl * FudgeRefCount;

FudgeStatus FudgeRefCount_create ( FudgeRefCount * refcountptr );
FudgeStatus FudgeRefCount_destroy ( FudgeRefCount refcount );

void FudgeRefCount_increment ( FudgeRefCount refcount );
int FudgeRefCount_decrementAndReturn ( FudgeRefCount refcount );
int FudgeRefCount_count ( FudgeRefCount refcount );

#ifdef __cplusplus
    }
#endif

#endif

