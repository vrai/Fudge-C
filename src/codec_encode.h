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
#ifndef INC_FUDGE_CODEC_ENCODE_H
#define INC_FUDGE_CODEC_ENCODE_H

#include "fudge/codec.h"

/* Registry compatible field data encoding functions: writes only the data,
   not the field header */
FudgeStatus FudgeCodec_encodeFieldIndicator ( const FudgeField * field, fudge_byte * * data );
FudgeStatus FudgeCodec_encodeFieldByte      ( const FudgeField * field, fudge_byte * * data );
FudgeStatus FudgeCodec_encodeFieldBool      ( const FudgeField * field, fudge_byte * * data );
FudgeStatus FudgeCodec_encodeFieldI16       ( const FudgeField * field, fudge_byte * * data );
FudgeStatus FudgeCodec_encodeFieldI32       ( const FudgeField * field, fudge_byte * * data );
FudgeStatus FudgeCodec_encodeFieldI64       ( const FudgeField * field, fudge_byte * * data );
FudgeStatus FudgeCodec_encodeFieldF32       ( const FudgeField * field, fudge_byte * * data );
FudgeStatus FudgeCodec_encodeFieldF64       ( const FudgeField * field, fudge_byte * * data );

FudgeStatus FudgeCodec_encodeFieldI16Array  ( const FudgeField * field, fudge_byte * * data );
FudgeStatus FudgeCodec_encodeFieldI32Array  ( const FudgeField * field, fudge_byte * * data );
FudgeStatus FudgeCodec_encodeFieldI64Array  ( const FudgeField * field, fudge_byte * * data );
FudgeStatus FudgeCodec_encodeFieldF32Array  ( const FudgeField * field, fudge_byte * * data );
FudgeStatus FudgeCodec_encodeFieldF64Array  ( const FudgeField * field, fudge_byte * * data );

FudgeStatus FudgeCodec_encodeFieldString    ( const FudgeField * field, fudge_byte * * data );

FudgeStatus FudgeCodec_encodeFieldFudgeMsg  ( const FudgeField * field, fudge_byte * * data );

#endif

