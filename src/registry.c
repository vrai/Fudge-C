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
#include "registry.h"
#include "codec_decode.h"
#include "codec_encode.h"

static fudge_bool s_registryInitialised = FUDGE_FALSE;
static FudgeTypeDesc s_registry [ FUDGE_REGISTRY_SIZE ];

void FudgeRegistry_registerType ( fudge_type_id type,
                                  fudge_i32 fixedwidth,
                                  FudgeTypePayload payload,
                                  FudgeTypeDecoder decoder,
                                  FudgeTypeEncoder encoder )
{
    FudgeTypeDesc * desc = &( s_registry [ type ] );
    desc->type = type;
    desc->fixedwidth = fixedwidth;
    desc->payload = payload;
    desc->decoder = decoder;
    desc->encoder = encoder;
}

FudgeStatus FudgeRegistry_init ( )
{
    int index;

    if ( s_registryInitialised )
        return FUDGE_OK;

    /* Make sure that the registry starts out clear */
    for ( index = 0; index < FUDGE_REGISTRY_SIZE; ++index )
        FudgeRegistry_registerType ( ( fudge_byte ) index, -1, FUDGE_TYPE_PAYLOAD_BYTES, 0, 0 );
 
    /* Register built in types */
    FudgeRegistry_registerType ( FUDGE_TYPE_INDICATOR,        0, FUDGE_TYPE_PAYLOAD_LOCAL,  FudgeCodec_decodeFieldIndicator,  FudgeCodec_encodeFieldIndicator );
    FudgeRegistry_registerType ( FUDGE_TYPE_BOOLEAN,          1, FUDGE_TYPE_PAYLOAD_LOCAL,  FudgeCodec_decodeFieldBool,       FudgeCodec_encodeFieldBool );
    FudgeRegistry_registerType ( FUDGE_TYPE_BYTE,             1, FUDGE_TYPE_PAYLOAD_LOCAL,  FudgeCodec_decodeFieldByte,       FudgeCodec_encodeFieldByte );
    FudgeRegistry_registerType ( FUDGE_TYPE_SHORT,            2, FUDGE_TYPE_PAYLOAD_LOCAL,  FudgeCodec_decodeFieldShort,      FudgeCodec_encodeFieldI16 );
    FudgeRegistry_registerType ( FUDGE_TYPE_INT,              4, FUDGE_TYPE_PAYLOAD_LOCAL,  FudgeCodec_decodeFieldInt,        FudgeCodec_encodeFieldI32 );
    FudgeRegistry_registerType ( FUDGE_TYPE_LONG,             8, FUDGE_TYPE_PAYLOAD_LOCAL,  FudgeCodec_decodeFieldLong,       FudgeCodec_encodeFieldI64 );
    FudgeRegistry_registerType ( FUDGE_TYPE_FLOAT,            4, FUDGE_TYPE_PAYLOAD_LOCAL,  FudgeCodec_decodeFieldFloat,      FudgeCodec_encodeFieldF32 );
    FudgeRegistry_registerType ( FUDGE_TYPE_DOUBLE,           8, FUDGE_TYPE_PAYLOAD_LOCAL,  FudgeCodec_decodeFieldDouble,     FudgeCodec_encodeFieldF64 );

    FudgeRegistry_registerType ( FUDGE_TYPE_BYTE_ARRAY_4,     4, FUDGE_TYPE_PAYLOAD_BYTES,  FudgeCodec_decodeFieldBytesArray, FudgeCodec_encodeFieldByteArray );
    FudgeRegistry_registerType ( FUDGE_TYPE_BYTE_ARRAY_8,     8, FUDGE_TYPE_PAYLOAD_BYTES,  FudgeCodec_decodeFieldBytesArray, FudgeCodec_encodeFieldByteArray );
    FudgeRegistry_registerType ( FUDGE_TYPE_BYTE_ARRAY_16,   16, FUDGE_TYPE_PAYLOAD_BYTES,  FudgeCodec_decodeFieldBytesArray, FudgeCodec_encodeFieldByteArray );
    FudgeRegistry_registerType ( FUDGE_TYPE_BYTE_ARRAY_20,   20, FUDGE_TYPE_PAYLOAD_BYTES,  FudgeCodec_decodeFieldBytesArray, FudgeCodec_encodeFieldByteArray );
    FudgeRegistry_registerType ( FUDGE_TYPE_BYTE_ARRAY_32,   32, FUDGE_TYPE_PAYLOAD_BYTES,  FudgeCodec_decodeFieldBytesArray, FudgeCodec_encodeFieldByteArray );
    FudgeRegistry_registerType ( FUDGE_TYPE_BYTE_ARRAY_64,   64, FUDGE_TYPE_PAYLOAD_BYTES,  FudgeCodec_decodeFieldBytesArray, FudgeCodec_encodeFieldByteArray );
    FudgeRegistry_registerType ( FUDGE_TYPE_BYTE_ARRAY_128, 128, FUDGE_TYPE_PAYLOAD_BYTES,  FudgeCodec_decodeFieldBytesArray, FudgeCodec_encodeFieldByteArray );
    FudgeRegistry_registerType ( FUDGE_TYPE_BYTE_ARRAY_256, 256, FUDGE_TYPE_PAYLOAD_BYTES,  FudgeCodec_decodeFieldBytesArray, FudgeCodec_encodeFieldByteArray );
    FudgeRegistry_registerType ( FUDGE_TYPE_BYTE_ARRAY_512, 512, FUDGE_TYPE_PAYLOAD_BYTES,  FudgeCodec_decodeFieldBytesArray, FudgeCodec_encodeFieldByteArray );
    
    FudgeRegistry_registerType ( FUDGE_TYPE_STRING,          -1, FUDGE_TYPE_PAYLOAD_BYTES,  FudgeCodec_decodeFieldBytesArray, FudgeCodec_encodeFieldByteArray );
    FudgeRegistry_registerType ( FUDGE_TYPE_BYTE_ARRAY,      -1, FUDGE_TYPE_PAYLOAD_BYTES,  FudgeCodec_decodeFieldBytesArray, FudgeCodec_encodeFieldByteArray );
    FudgeRegistry_registerType ( FUDGE_TYPE_SHORT_ARRAY,     -1, FUDGE_TYPE_PAYLOAD_BYTES,  FudgeCodec_decodeFieldI16Array,   FudgeCodec_encodeFieldI16Array );
    FudgeRegistry_registerType ( FUDGE_TYPE_INT_ARRAY,       -1, FUDGE_TYPE_PAYLOAD_BYTES,  FudgeCodec_decodeFieldI32Array,   FudgeCodec_encodeFieldI32Array );
    FudgeRegistry_registerType ( FUDGE_TYPE_LONG_ARRAY,      -1, FUDGE_TYPE_PAYLOAD_BYTES,  FudgeCodec_decodeFieldI64Array,   FudgeCodec_encodeFieldI64Array );
    FudgeRegistry_registerType ( FUDGE_TYPE_FLOAT_ARRAY,     -1, FUDGE_TYPE_PAYLOAD_BYTES,  FudgeCodec_decodeFieldF32Array,   FudgeCodec_encodeFieldF32Array );
    FudgeRegistry_registerType ( FUDGE_TYPE_DOUBLE_ARRAY,    -1, FUDGE_TYPE_PAYLOAD_BYTES,  FudgeCodec_decodeFieldF64Array,   FudgeCodec_encodeFieldF64Array );

    FudgeRegistry_registerType ( FUDGE_TYPE_FUDGE_MSG,       -1, FUDGE_TYPE_PAYLOAD_SUBMSG, FudgeCodec_decodeFieldFudgeMsg,   FudgeCodec_encodeFieldFudgeMsg );

    s_registryInitialised = FUDGE_TRUE;
    return FUDGE_OK;
}

const FudgeTypeDesc * FudgeRegistry_getTypeDesc ( fudge_type_id type )
{
    return &( s_registry [ type ] );
}

