/**
 * Copyright (C) 2009 - 20010, Vrai Stacey.
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
        case FUDGE_OK:                            return "OK";
        case FUDGE_OUT_OF_MEMORY:                 return "Out of Memory";
        case FUDGE_NULL_POINTER:                  return "Null Pointer";
        case FUDGE_INVALID_INDEX:                 return "Invalid Index";
        case FUDGE_INVALID_NAME:                  return "Invalid Name";
        case FUDGE_INVALID_ORDINAL:               return "Invalid Ordinal";
        case FUDGE_NAME_TOO_LONG:                 return "Name Too Long";
        case FUDGE_INVALID_TYPE_COERCION:         return "Invaild Type Coercion";
        case FUDGE_COERCION_NOT_REQUIRED:         return "Coercion Not Required";
        case FUDGE_INVALID_TYPE_ACCESSOR:         return "Cannot use this accessor/field type combination";
        case FUDGE_INVALID_USER_TYPE:             return "Invalid User Type";
        case FUDGE_STRING_INVALID_ASCII:          return "Invalid ASCII char in string";
        case FUDGE_STRING_INVALID_UNICODE:        return "Invalid Unicode char encountered";
        case FUDGE_STRING_INCOMPLETE_UNICODE:     return "Incomplete Unicode char encountered";
        case FUDGE_STRING_UNCONSUMED_UNICODE:     return "Unicode conversion did not consume all source chars";
        case FUDGE_STRING_INVALID_SURROGATE:      return "Invalid 3-byte modified UTF8 sequence";
        case FUDGE_STRING_UNKNOWN_UNICODE_TYPE:   return "Unicode type was not recognised";
        case FUDGE_OUT_OF_BYTES:                  return "Out of Bytes";
        case FUDGE_UNKNOWN_FIELD_WIDTH:           return "Unknown Field Width";
        case FUDGE_DATETIME_INVALID_YEAR:         return "Invalid value for Year";
        case FUDGE_DATETIME_INVALID_MONTH:        return "Invalid value for Month";
        case FUDGE_DATETIME_INVALID_DAY:          return "Invalid value for Day of Month";
        case FUDGE_DATETIME_INVALID_SECONDS:      return "Invalid value for Seconds since midnight";
        case FUDGE_DATETIME_INVALID_NANOSECONDS:  return "Invalid value for Nanoseconds since start of second";
        case FUDGE_INTERNAL_LIST_STATE:           return "Internal List State";
        case FUDGE_INTERNAL_PAYLOAD:              return "Internal Type Payload Is Invalid";
        case FUDGE_REGISTRY_UNINITIALISED:        return "Fudge Registry Not Initialised";
        default:                                  return "Unknown Status";
    }
}

