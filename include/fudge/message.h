#ifndef INC_FUDGE_MESSAGE_H
#define INC_FUDGE_MESSAGE_H

#include "fudge/status.h"
#include "fudge/types.h"

#ifdef __cplusplus
    extern "C" {
#endif

FudgeStatus FudgeMsg_create ( FudgeMsg * messageptr );
FudgeStatus FudgeMsg_retain ( FudgeMsg message );
FudgeStatus FudgeMsg_release ( FudgeMsg message );

unsigned long FudgeMsg_numFields ( FudgeMsg message );

FudgeStatus FudgeMsg_addFieldMsg ( FudgeMsg message, const char * name, FudgeMsg value );

#ifdef __cplusplus
    }
#endif

#endif

