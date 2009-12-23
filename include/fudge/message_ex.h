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
#ifndef INC_FUDGE_MESSAGE_EX_H
#define INC_FUDGE_MESSAGE_EX_H

#include "fudge/status.h"
#include "fudge/types.h"

#ifdef __cplusplus
    extern "C" {
#endif

/* Extended Fudge access functions for FudgeMsg objects and their fields. For
   additional notes on memory handling and thread safety, see message.h */

/* Primitive type conversion functions. Will attempt to convert the field
   provided in to the requested type. Returns FUDGE_OK on success,
   FUDGE_INVALID_TYPE_COERCION if the conversion is not valid and
   FUDGE_NULL_POINTER if the arguments provided are NULL. */
FUDGEAPI FudgeStatus FudgeMsg_getFieldAsBoolean ( const FudgeField * field, fudge_bool * target );
FUDGEAPI FudgeStatus FudgeMsg_getFieldAsByte    ( const FudgeField * field, fudge_byte * target );
FUDGEAPI FudgeStatus FudgeMsg_getFieldAsI16     ( const FudgeField * field, fudge_i16 * target );
FUDGEAPI FudgeStatus FudgeMsg_getFieldAsI32     ( const FudgeField * field, fudge_i32 * target );
FUDGEAPI FudgeStatus FudgeMsg_getFieldAsI64     ( const FudgeField * field, fudge_i64 * target );
FUDGEAPI FudgeStatus FudgeMsg_getFieldAsF32     ( const FudgeField * field, fudge_f32 * target );
FUDGEAPI FudgeStatus FudgeMsg_getFieldAsF64     ( const FudgeField * field, fudge_f64 * target );

#ifdef __cplusplus
    }
#endif

#endif

