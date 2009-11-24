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
#ifndef INC_FUDGE_STATUS_H
#define INC_FUDGE_STATUS_H

#ifdef __cplusplus
    extern "C" {
#endif

typedef enum
{
    FUDGE_OK                    = 0x0000,
    FUDGE_OUT_OF_MEMORY         = 0x0001,
    FUDGE_NULL_POINTER          = 0x0002,
    FUDGE_NAME_TOO_LONG         = 0x0003,
    FUDGE_INVALID_INDEX         = 0x0010,
    FUDGE_OUT_OF_BYTES          = 0x0100,
    FUDGE_UNKNOWN_FIELD_WIDTH   = 0x0101,
    FUDGE_INTERNAL_LIST_STATE   = 0x1000

} FudgeStatus;

extern const char * FudgeStatus_strerror ( FudgeStatus status );

#ifdef __cplusplus
    }
#endif

#endif

