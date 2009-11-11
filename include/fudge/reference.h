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

