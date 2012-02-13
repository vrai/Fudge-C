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
#include "reference.h"
#include "memory_internal.h"
#include "fudge/platform.h"
#include <assert.h>

struct FudgeRefCountImpl
{
    int count;
};

FudgeStatus FudgeRefCount_create ( FudgeRefCount * refcountptr )
{
    if ( ! ( *refcountptr = FUDGEMEMORY_MALLOC( FudgeRefCount, sizeof ( struct FudgeRefCountImpl ) ) ) )
        return FUDGE_OUT_OF_MEMORY;

    ( *refcountptr )->count = 1u;

    return FUDGE_OK;
}

FudgeStatus FudgeRefCount_destroy ( FudgeRefCount refcount )
{
    FUDGEMEMORY_FREE( refcount );
    return FUDGE_OK;
}

void FudgeRefCount_increment ( FudgeRefCount refcount )
{
    if ( refcount )
	    refcount->count += 1u;
    else
        assert ( refcount );
}

int FudgeRefCount_decrementAndReturn ( FudgeRefCount refcount )
{
    if ( refcount )
    {
        assert ( refcount->count >= 1u );
        return refcount->count -= 1u;
    }
    else
    {
        assert ( refcount );
        return 0u;
    }
}

int FudgeRefCount_count ( FudgeRefCount refcount )
{
    if ( refcount )
        return refcount->count;
    else
    {
        assert ( refcount );
        return 0u;
    }
}

