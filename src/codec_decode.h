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
#ifndef INC_FUDGE_CODEC_DECODE_H
#define INC_FUDGE_CODEC_DECODE_H

#include "fudge/codec.h"

extern FudgeStatus FudgeCodec_decodeFieldIndicator  ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
extern FudgeStatus FudgeCodec_decodeFieldBool       ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
extern FudgeStatus FudgeCodec_decodeFieldByte       ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
extern FudgeStatus FudgeCodec_decodeFieldShort      ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
extern FudgeStatus FudgeCodec_decodeFieldInt        ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
extern FudgeStatus FudgeCodec_decodeFieldLong       ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
extern FudgeStatus FudgeCodec_decodeFieldFloat      ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
extern FudgeStatus FudgeCodec_decodeFieldDouble     ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );

extern FudgeStatus FudgeCodec_decodeFieldBytesArray ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
extern FudgeStatus FudgeCodec_decodeFieldI16Array   ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
extern FudgeStatus FudgeCodec_decodeFieldI32Array   ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
extern FudgeStatus FudgeCodec_decodeFieldI64Array   ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
extern FudgeStatus FudgeCodec_decodeFieldF32Array   ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );
extern FudgeStatus FudgeCodec_decodeFieldF64Array   ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );

extern FudgeStatus FudgeCodec_decodeFieldFudgeMsg   ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );

#endif

