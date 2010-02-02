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
#ifndef INC_FUDGE_CODEC_EX_H
#define INC_FUDGE_CODEC_EX_H

#include "fudge/message.h"

#ifdef __cplusplus
    extern "C" {
#endif

/* Field length encoding function: writes the length using the minimum number
   of bytes required. */
FUDGEAPI void FudgeCodec_encodeFieldLength ( const fudge_i32 length, fudge_byte * * data );

/* Field data encoding functions: writes only the data, not the field header */
FUDGEAPI void FudgeCodec_encodeByte ( const fudge_byte value, fudge_byte * * data );
FUDGEAPI void FudgeCodec_encodeI16  ( fudge_i16 value,  fudge_byte * * data );
FUDGEAPI void FudgeCodec_encodeI32  ( fudge_i32 value,  fudge_byte * * data );
FUDGEAPI void FudgeCodec_encodeI64  ( fudge_i64 value,  fudge_byte * * data );
FUDGEAPI void FudgeCodec_encodeF32  ( fudge_f32 value,  fudge_byte * * data );
FUDGEAPI void FudgeCodec_encodeF64  ( fudge_f64 value,  fudge_byte * * data );

/* Copies the contents of the byte array to the target data pointer. If the
   array is not fixed width, the contents are prefixed with the array width,
   written using FudgeCodec_encodeFieldLength (see above). */
FUDGEAPI void FudgeCodec_encodeByteArray ( const fudge_byte * bytes,
                                           const fudge_i32 width,
                                           const fudge_bool fixedwidth,
                                           fudge_byte * * data );

/* Field data decoding functions: reads only the data */
FUDGEAPI fudge_bool FudgeCodec_decodeBool ( const fudge_byte * bytes );
FUDGEAPI fudge_byte FudgeCodec_decodeByte ( const fudge_byte * bytes );
FUDGEAPI fudge_i16  FudgeCodec_decodeI16  ( const fudge_byte * bytes );
FUDGEAPI fudge_i32  FudgeCodec_decodeI32  ( const fudge_byte * bytes );
FUDGEAPI fudge_i64  FudgeCodec_decodeI64  ( const fudge_byte * bytes );
FUDGEAPI fudge_f32  FudgeCodec_decodeF32  ( const fudge_byte * bytes );
FUDGEAPI fudge_f64  FudgeCodec_decodeF64  ( const fudge_byte * bytes );

/* Field data decoding functions: reads only the data */

/* Creates a copy of the bytes required and points *bytes at it. If the width
   is zero, no memory is allocated and *bytes is set to zero. */
FUDGEAPI FudgeStatus FudgeCodec_decodeByteArray ( const fudge_byte * data,
                                                  const fudge_i32 width,
                                                  fudge_byte * * bytes );

/* Registry compatible byte array encoding/decoding functions. For use by
   user types that can be represented as simple byte arrays. */
FUDGEAPI FudgeStatus FudgeCodec_encodeFieldByteArray ( const FudgeField * field, fudge_byte * * data );
FUDGEAPI FudgeStatus FudgeCodec_decodeFieldByteArray ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );

#ifdef __cplusplus
    }
#endif

#endif

