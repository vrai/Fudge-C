#include "reference.h"
#include "fudge/platform.h"
#include <assert.h>

struct FudgeRefCountImpl
{
    int count;     /* TODO Make atomic */
};

FudgeStatus FudgeRefCount_create ( FudgeRefCount * refcountptr )
{
    if ( ! ( *refcountptr = ( FudgeRefCount ) malloc ( sizeof ( struct FudgeRefCountImpl ) ) ) )
        return FUDGE_OUT_OF_MEMORY;

    ( *refcountptr )->count = 1u;

    return FUDGE_OK;
}

FudgeStatus FudgeRefCount_destroy ( FudgeRefCount refcount )
{
    free ( refcount );
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

