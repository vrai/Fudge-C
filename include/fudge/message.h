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
#ifndef INC_FUDGE_MESSAGE_H
#define INC_FUDGE_MESSAGE_H

#include "fudge/status.h"
#include "fudge/types.h"

#ifdef __cplusplus
    extern "C" {
#endif

extern FudgeStatus FudgeMsg_create ( FudgeMsg * messageptr );
extern FudgeStatus FudgeMsg_retain ( FudgeMsg message );
extern FudgeStatus FudgeMsg_release ( FudgeMsg message );

extern unsigned long FudgeMsg_numFields ( FudgeMsg message );

extern FudgeStatus FudgeMsg_addFieldIndicator ( FudgeMsg message, const char * name, const fudge_i16 * ordinal );

extern FudgeStatus FudgeMsg_addFieldBool ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, fudge_bool value );
extern FudgeStatus FudgeMsg_addFieldByte ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, fudge_byte value );
extern FudgeStatus FudgeMsg_addFieldI16  ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, fudge_i16 value );
extern FudgeStatus FudgeMsg_addFieldI32  ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, fudge_i32 value );
extern FudgeStatus FudgeMsg_addFieldI64  ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, fudge_i64 value );
extern FudgeStatus FudgeMsg_addFieldF32  ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, fudge_f32 value );
extern FudgeStatus FudgeMsg_addFieldF64  ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, fudge_f64 value );

extern FudgeStatus FudgeMsg_addFieldMsg ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, FudgeMsg value );

extern FudgeStatus FudgeMsg_addFieldByteArray ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, const fudge_byte * bytes,  fudge_i32 numbytes );
extern FudgeStatus FudgeMsg_addFieldI16Array  ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, const fudge_i16 * ints,    fudge_i32 numints );
extern FudgeStatus FudgeMsg_addFieldI32Array  ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, const fudge_i32 * ints,    fudge_i32 numints );
extern FudgeStatus FudgeMsg_addFieldI64Array  ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, const fudge_i64 * ints,    fudge_i32 numints );
extern FudgeStatus FudgeMsg_addFieldF32Array  ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, const fudge_f32 * floats,  fudge_i32 numfloats );
extern FudgeStatus FudgeMsg_addFieldF64Array  ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, const fudge_f64 * doubles, fudge_i32 numdoubles );

extern FudgeStatus FudgeMsg_addFieldString ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, const char * string, fudge_i32 numbytes );

extern FudgeStatus FudgeMsg_addField4ByteArray   ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, const fudge_byte * bytes );
extern FudgeStatus FudgeMsg_addField8ByteArray   ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, const fudge_byte * bytes );
extern FudgeStatus FudgeMsg_addField16ByteArray  ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, const fudge_byte * bytes );
extern FudgeStatus FudgeMsg_addField20ByteArray  ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, const fudge_byte * bytes );
extern FudgeStatus FudgeMsg_addField32ByteArray  ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, const fudge_byte * bytes );
extern FudgeStatus FudgeMsg_addField64ByteArray  ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, const fudge_byte * bytes );
extern FudgeStatus FudgeMsg_addField128ByteArray ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, const fudge_byte * bytes );
extern FudgeStatus FudgeMsg_addField256ByteArray ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, const fudge_byte * bytes );
extern FudgeStatus FudgeMsg_addField512ByteArray ( FudgeMsg message, const char * name, const fudge_i16 * ordinal, const fudge_byte * bytes );

extern FudgeStatus FudgeMsg_addFieldOpaque ( FudgeMsg message,
                                             fudge_type_id type,
                                             const char * name,
                                             const fudge_i16 * ordinal,
                                             const fudge_byte * bytes,
                                             fudge_i32 numbytes );

extern FudgeStatus FudgeMsg_getFieldAtIndex ( FudgeField * field, FudgeMsg message, unsigned long index );

#ifdef __cplusplus
    }
#endif

#endif

