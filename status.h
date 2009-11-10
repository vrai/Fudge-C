#ifndef INC_FUDGE_STATUS_H
#define INC_FUDGE_STATUS_H

#ifdef __cplusplus
    extern "C" {
#endif

typedef enum
{
    FUDGE_OK                    = 0x0,
    FUDGE_OUT_OF_MEMORY         = 0x1,
    FUDGE_NULL_POINTER          = 0x2,
    FUDGE_INTERNAL_LIST_STATE   = 0x3

} FudgeStatus;

extern const char * FudgeStatus_strerror ( FudgeStatus status );

#ifdef __cplusplus
    }
#endif

#endif

