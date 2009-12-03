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
#include "fudge/status.h"

const char * FudgeStatus_strerror ( FudgeStatus status )
{
    switch ( status )
    {
        case FUDGE_OK:                     return "OK";
        case FUDGE_OUT_OF_MEMORY:          return "Out of Memory";
        case FUDGE_NULL_POINTER:           return "Null Pointer";
        case FUDGE_INVALID_INDEX:          return "Invalid Index";
        case FUDGE_INVALID_NAME:           return "Invalid Name";
        case FUDGE_INVALID_ORDINAL:        return "Invalid Ordinal";
        case FUDGE_NAME_TOO_LONG:          return "Name Too Long";
        case FUDGE_OUT_OF_BYTES:           return "Out of Bytes";
        case FUDGE_UNKNOWN_FIELD_WIDTH:    return "Unknown Field Width";
        case FUDGE_INTERNAL_LIST_STATE:    return "Internal List State";
        case FUDGE_INTERNAL_PAYLOAD:       return "Internal Type Payload Is Invalid";
        case FUDGE_REGISTRY_UNINITIALISED: return "Fudge Registry Not Initialised";
        default:                           return "Unknown Status";
    }
}

