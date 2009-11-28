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
#ifndef INC_CODEC_ENCODE_H
#define INC_CODEC_ENCODE_H

#include "fudge/codec.h"

extern void FudgeCodec_encodeFieldByte ( fudge_byte value, fudge_byte * * writepos );
extern void FudgeCodec_encodeFieldI16  ( fudge_i16 value,  fudge_byte * * writepos );
extern void FudgeCodec_encodeFieldI32  ( fudge_i32 value,  fudge_byte * * writepos );
extern void FudgeCodec_encodeFieldI64  ( fudge_i64 value,  fudge_byte * * writepos );
extern void FudgeCodec_encodeFieldF32  ( fudge_f32 value,  fudge_byte * * writepos );
extern void FudgeCodec_encodeFieldF64  ( fudge_f64 value,  fudge_byte * * writepos );

#endif

