#include "fudge/status.h"

const char * FudgeStatus_strerror ( FudgeStatus status )
{
    switch ( status )
    {
        case FUDGE_OK:                  return "OK";
        case FUDGE_OUT_OF_MEMORY:       return "Out Of Memory";
        case FUDGE_NULL_POINTER:        return "Null Pointer";
        case FUDGE_INVALID_INDEX:       return "Invalid Index";
        case FUDGE_INTERNAL_LIST_STATE: return "Internal List State";
        default:                        return "Unknown Status";
    }
}

