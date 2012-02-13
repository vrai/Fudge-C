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
#ifndef INC_FUDGE_STATUS_H
#define INC_FUDGE_STATUS_H

#include "fudgeapi.h"

#ifdef __cplusplus
    extern "C" {
#endif

typedef enum
{
    FUDGE_OK                            = 0x0000,
    FUDGE_OUT_OF_MEMORY                 = 0x0001,
    FUDGE_NULL_POINTER                  = 0x0002,

    FUDGE_INVALID_INDEX                 = 0x0010,
    FUDGE_INVALID_NAME                  = 0x0011,
    FUDGE_INVALID_ORDINAL               = 0x0012,
    FUDGE_NAME_TOO_LONG                 = 0x0013,
    FUDGE_PAYLOAD_TOO_LONG              = 0x0014,

    FUDGE_INVALID_TYPE_COERCION         = 0x0020,
    FUDGE_COERCION_NOT_REQUIRED         = 0x0021,
    FUDGE_INVALID_TYPE_ACCESSOR         = 0x0022,

    FUDGE_INVALID_USER_TYPE             = 0x0040,

    FUDGE_STRING_INVALID_ASCII          = 0x0080,
    FUDGE_STRING_INVALID_UNICODE        = 0x0081,
    FUDGE_STRING_INCOMPLETE_UNICODE     = 0x0082,
    FUDGE_STRING_UNCONSUMED_UNICODE     = 0x0083,
    FUDGE_STRING_INVALID_SURROGATE      = 0x0084,
    FUDGE_STRING_UNKNOWN_UNICODE_TYPE   = 0x0085,

    FUDGE_OUT_OF_BYTES                  = 0x0100,
    FUDGE_UNKNOWN_FIELD_WIDTH           = 0x0101,

    FUDGE_DATETIME_INVALID_YEAR         = 0x0200,
    FUDGE_DATETIME_INVALID_MONTH        = 0x0201,
    FUDGE_DATETIME_INVALID_DAY          = 0x0202,
    FUDGE_DATETIME_INVALID_SECONDS      = 0x0203,
    FUDGE_DATETIME_INVALID_NANOSECONDS  = 0x0204,

    FUDGE_PTHREAD_MUTEX_RESOURCES       = 0x0300,
    FUDGE_PTHREAD_MUTEX_BUSY            = 0x0301,
    FUDGE_PTHREAD_MUTEX_INVALID         = 0x0302,
    FUDGE_PTHREAD_MUTEX_UNKNOWN         = 0x0303,

    FUDGE_INTERNAL_LIST_STATE           = 0x1000,
    FUDGE_INTERNAL_PAYLOAD              = 0x1001,

    FUDGE_REGISTRY_UNINITIALISED        = 0x2000,
    FUDGE_CHANGED_MEMORY_MANAGER        = 0x2001

} FudgeStatus;

/* Returns a reference to a constant static string containing the error
   message for the status provided. Returns a standard error if the status 
   is unknown. */
FUDGEAPI const char * FudgeStatus_strerror ( FudgeStatus status );

#ifdef __cplusplus
    }
#endif

#endif

