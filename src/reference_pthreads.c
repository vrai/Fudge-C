/**
 * Copyright (C) 2009 - 2010, Vrai Stacey.
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
#include "errno.h"
#include "fudge/platform.h"
#include <pthread.h>
#include <assert.h>

static inline FudgeStatus Reference_pthreadResultToFudgeStatus ( int result )
{
    switch ( result )
    {
        case 0:      return FUDGE_OK;
        case EAGAIN: return FUDGE_PTHREAD_MUTEX_RESOURCES;
        case ENOMEM: return FUDGE_OUT_OF_MEMORY;
        case EBUSY:  return FUDGE_PTHREAD_MUTEX_BUSY;
        case EINVAL: return FUDGE_PTHREAD_MUTEX_INVALID;
        default:     return FUDGE_PTHREAD_MUTEX_UNKNOWN;
    }
}

struct FudgeRefCountImpl
{
    pthread_mutex_t mutex;
    int count;
};

FudgeStatus FudgeRefCount_create ( FudgeRefCount * refcountptr )
{
    int result;

    if ( ! ( *refcountptr = ( FudgeRefCount ) malloc ( sizeof ( struct FudgeRefCountImpl ) ) ) )
        return FUDGE_OUT_OF_MEMORY;

    if ( ( result = pthread_mutex_init ( &( ( *refcountptr )->mutex ), NULL ) ) )
    {
        free ( *refcountptr );
        return Reference_pthreadResultToFudgeStatus ( result );
    }

    ( *refcountptr )->count = 1u;
    return FUDGE_OK;
}

FudgeStatus FudgeRefCount_destroy ( FudgeRefCount refcount )
{
    int result;
    
    result = pthread_mutex_destroy ( &( refcount->mutex ) );
    free ( refcount );
    return Reference_pthreadResultToFudgeStatus ( result );
}

void FudgeRefCount_increment ( FudgeRefCount refcount )
{
    pthread_mutex_lock ( &( refcount->mutex ) );
    if ( refcount )
        refcount->count += 1u;
    else
        assert ( refcount );
    pthread_mutex_unlock ( &( refcount->mutex ) );
}

int FudgeRefCount_decrementAndReturn ( FudgeRefCount refcount )
{
    int count;

    pthread_mutex_lock ( &( refcount->mutex ) );
    if ( refcount )
    {
        assert ( refcount->count >= 1u );
        count = refcount->count -= 1u;
    }
    else
    {
        assert ( refcount );
        count = 0u;
    }
    pthread_mutex_unlock ( &( refcount->mutex ) );

    return count;
}

int FudgeRefCount_count ( FudgeRefCount refcount )
{
    int count;

    pthread_mutex_lock ( &( refcount->mutex ) );
    if ( refcount )
        count = refcount->count;
    else
    {
        assert ( refcount );
        count = 0u;
    }
    pthread_mutex_unlock ( &( refcount->mutex ) );

    return count;
}

