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
#include "fudge/stringpool.h"
#include "reference.h"

typedef struct StringPoolNode
{
    FudgeString string;
    struct StringPoolNode * next;
} StringPoolNode;

struct FudgeStringPoolImpl
{
    FudgeRefCount refcount;
    StringPoolNode * stringhead;
};

FudgeStatus FudgeStringPool_create ( FudgeStringPool * pool )
{
    FudgeStatus status;

    if ( ! pool )
        return FUDGE_NULL_POINTER;

    if ( ! ( *pool = ( FudgeStringPool ) malloc ( sizeof ( struct FudgeStringPoolImpl ) ) ) )
        return FUDGE_OUT_OF_MEMORY;

    ( *pool )->stringhead = 0;

    if ( ( status = FudgeRefCount_create ( &( ( *pool )->refcount ) ) ) != FUDGE_OK )
        free ( pool );

    return status;
}

FudgeStatus FudgeStringPool_clear ( FudgeStringPool pool )
{
    StringPoolNode * node;

    if ( ! pool )
        return FUDGE_NULL_POINTER;

    while ( ( node = pool->stringhead ) )
    {
        FudgeString_release ( node->string );
        pool->stringhead = node->next;
        free ( node );
    }

    return FUDGE_OK;
}

void FudgeStringPool_destroy ( FudgeStringPool pool )
{
    if ( pool )
    {
        FudgeStringPool_clear ( pool );
        FudgeRefCount_destroy ( pool->refcount );
        free ( pool );
    }
}

FudgeStatus FudgeStringPool_retain ( FudgeStringPool pool )
{
    if ( ! pool )
        return FUDGE_NULL_POINTER;

    FudgeRefCount_increment ( pool->refcount );
    return FUDGE_OK;
}

FudgeStatus FudgeStringPool_release ( FudgeStringPool pool )
{
    if ( ! pool )
        return FUDGE_NULL_POINTER;

    if ( ! FudgeRefCount_decrementAndReturn ( pool->refcount ) )
        FudgeStringPool_destroy ( pool );
    return FUDGE_OK;
}

FudgeStatus FudgeStringPool_acquire ( FudgeStringPool pool, FudgeString string )
{
    StringPoolNode * node;

    if ( ! ( pool && string ) )
        return FUDGE_NULL_POINTER;

    if ( ! ( node = ( StringPoolNode * ) malloc ( sizeof ( StringPoolNode )) ) )
        return FUDGE_OUT_OF_MEMORY;

    node->string = string;
    node->next = pool->stringhead ? pool->stringhead : 0;

    pool->stringhead = node;
    return FUDGE_OK;
}

/* Used by the create+acquire functions, once the string has been successfully
   created. Handles the acquire failure case so can be immediately returned
   from in the calling function. */
FudgeString FudgeStringPool_acquireInternal ( FudgeStringPool pool, FudgeString string, FudgeStatus * status )
{
    FudgeStatus localstatus;

    if ( ( localstatus = FudgeStringPool_acquire ( pool, string ) ) != FUDGE_OK )
    {
        FudgeString_release ( string );
        string = 0;
    }

    if ( status )
        *status = localstatus;
    return string;
}

FudgeString FudgeStringPool_createStringFromASCII ( FudgeStringPool pool, FudgeStatus * status, const char * chars, size_t numchars )
{
    FudgeStatus localstatus;
    FudgeString string;

    if ( ! pool )
    {
        localstatus = FUDGE_NULL_POINTER;
        goto status_and_return;
    }

    if ( ( localstatus = FudgeString_createFromASCII ( &string, chars, numchars ) ) != FUDGE_OK )
    {
        string = 0;
        goto status_and_return;
    }

    return FudgeStringPool_acquireInternal ( pool, string, status );

status_and_return:
    if ( status )
        *status = localstatus;
    return string;
}

FudgeString FudgeStringPool_createStringFromASCIIZ ( FudgeStringPool pool, FudgeStatus * status, const char * chars )
{
    FudgeStatus localstatus;
    FudgeString string;

    if ( ! pool )
    {
        localstatus = FUDGE_NULL_POINTER;
        goto status_and_return;
    }

    if ( ( localstatus = FudgeString_createFromASCIIZ ( &string, chars ) ) != FUDGE_OK )
    {
        string = 0;
        goto status_and_return;
    }

    return FudgeStringPool_acquireInternal ( pool, string, status );

status_and_return:
    if ( status )
        *status = localstatus;
    return string;
}

