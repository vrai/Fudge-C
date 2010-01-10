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
#ifndef INC_CODEC_FIELD_H
#define INC_CODEC_FIELD_H

#include "fudge/codec.h"

/* Field length encoding function: writes the length using the minimum number
   of bytes required. */
FUDGEAPI void FudgeCodec_encodeFieldLength ( const fudge_i32 length, fudge_byte * * data );

/* Field encoding functions */
FUDGEAPI FudgeStatus FudgeCodec_encodeFieldIndicator ( const FudgeField * field, fudge_byte * * data );
FUDGEAPI FudgeStatus FudgeCodec_encodeFieldByte      ( const FudgeField * field, fudge_byte * * data );
FUDGEAPI FudgeStatus FudgeCodec_encodeFieldBool      ( const FudgeField * field, fudge_byte * * data );
FUDGEAPI FudgeStatus FudgeCodec_encodeFieldI16       ( const FudgeField * field, fudge_byte * * data );
FUDGEAPI FudgeStatus FudgeCodec_encodeFieldI32       ( const FudgeField * field, fudge_byte * * data );
FUDGEAPI FudgeStatus FudgeCodec_encodeFieldI64       ( const FudgeField * field, fudge_byte * * data );
FUDGEAPI FudgeStatus FudgeCodec_encodeFieldF32       ( const FudgeField * field, fudge_byte * * data );
FUDGEAPI FudgeStatus FudgeCodec_encodeFieldF64       ( const FudgeField * field, fudge_byte * * data );

FUDGEAPI FudgeStatus FudgeCodec_encodeFieldByteArray ( const FudgeField * field, fudge_byte * * data );
FUDGEAPI FudgeStatus FudgeCodec_encodeFieldI16Array  ( const FudgeField * field, fudge_byte * * data );
FUDGEAPI FudgeStatus FudgeCodec_encodeFieldI32Array  ( const FudgeField * field, fudge_byte * * data );
FUDGEAPI FudgeStatus FudgeCodec_encodeFieldI64Array  ( const FudgeField * field, fudge_byte * * data );
FUDGEAPI FudgeStatus FudgeCodec_encodeFieldF32Array  ( const FudgeField * field, fudge_byte * * data );
FUDGEAPI FudgeStatus FudgeCodec_encodeFieldF64Array  ( const FudgeField * field, fudge_byte * * data );

FUDGEAPI FudgeStatus FudgeCodec_encodeFieldFudgeMsg  ( const FudgeField * field, fudge_byte * * data );

/* Type encoding functions */
FUDGEAPI void FudgeCodec_encodeByte ( const fudge_byte value, fudge_byte * * data );
FUDGEAPI void FudgeCodec_encodeI16  ( fudge_i16 value,  fudge_byte * * data );
FUDGEAPI void FudgeCodec_encodeI32  ( fudge_i32 value,  fudge_byte * * data );
FUDGEAPI void FudgeCodec_encodeI64  ( fudge_i64 value,  fudge_byte * * data );
FUDGEAPI void FudgeCodec_encodeF32  ( fudge_f32 value,  fudge_byte * * data );
FUDGEAPI void FudgeCodec_encodeF64  ( fudge_f64 value,  fudge_byte * * data );

FUDGEAPI void FudgeCodec_encodeByteArray ( const fudge_byte * bytes,
                                         const fudge_i32 width,
                                         const fudge_bool fixedwidth,
                                         fudge_byte * * data );

/* Field decoding functions */
FUDGEAPI FudgeStatus FudgeCodec_decodeFieldIndicator  ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
FUDGEAPI FudgeStatus FudgeCodec_decodeFieldBool       ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
FUDGEAPI FudgeStatus FudgeCodec_decodeFieldByte       ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
FUDGEAPI FudgeStatus FudgeCodec_decodeFieldShort      ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
FUDGEAPI FudgeStatus FudgeCodec_decodeFieldInt        ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
FUDGEAPI FudgeStatus FudgeCodec_decodeFieldLong       ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
FUDGEAPI FudgeStatus FudgeCodec_decodeFieldFloat      ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
FUDGEAPI FudgeStatus FudgeCodec_decodeFieldDouble     ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );

FUDGEAPI FudgeStatus FudgeCodec_decodeFieldBytesArray ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
FUDGEAPI FudgeStatus FudgeCodec_decodeFieldI16Array   ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
FUDGEAPI FudgeStatus FudgeCodec_decodeFieldI32Array   ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
FUDGEAPI FudgeStatus FudgeCodec_decodeFieldI64Array   ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
FUDGEAPI FudgeStatus FudgeCodec_decodeFieldF32Array   ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
FUDGEAPI FudgeStatus FudgeCodec_decodeFieldF64Array   ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );

FUDGEAPI FudgeStatus FudgeCodec_decodeFieldFudgeMsg   ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );

#endif

