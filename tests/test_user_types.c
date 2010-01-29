/**
 * Copyright (C) 2009 - 2010 Vrai Stacey.
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
#include "simpletest.h"
#include "fudge/message.h"
#include "fudge/codec.h"

/* Example user types */

#define FUDGE_TYPE_EXAMPLEIP4   100
#define FUDGE_TYPE_EXAMPLETICK  101

typedef struct
{
    uint8_t quads [ 4 ];
} ExampleIp4Struct;

typedef struct
{
    char ric [ 16 ];
    fudge_f32 bid, ask;
    fudge_i32 time;
} ExampleTickStruct;

/* ExampleIP4 functions */

void ExampleIp4_init ( ExampleIp4Struct * ip4, uint8_t first, uint8_t second, uint8_t third, uint8_t fourth )
{
    ip4->quads [ 0 ] = first;
    ip4->quads [ 1 ] = second;
    ip4->quads [ 2 ] = third;
    ip4->quads [ 3 ] = fourth;
}

FudgeStatus FudgeMsg_addFieldExampleIp4 ( FudgeMsg message, const fudge_byte * name, fudge_i32 namelen, const ExampleIp4Struct * ip )
{
    if ( ! ( message && ip ) )
        return FUDGE_NULL_POINTER;

    return FudgeMsg_addFieldOpaque ( message,
                                     FUDGE_TYPE_EXAMPLEIP4,
                                     name,
                                     namelen,
                                     0,
                                     ( const fudge_byte * ) ip,
                                     sizeof ( ExampleIp4Struct ) );
}

FudgeStatus FudgeType_coerceExampleIp4 ( const FudgeField * source, const fudge_type_id type, FudgeFieldData * target, fudge_i32 * numbytes )
{
    if ( ! ( source && target && numbytes ) )
        return FUDGE_NULL_POINTER;
    if ( source->type == type )
        return FUDGE_COERCION_NOT_REQUIRED;

    /* For test purposes, only a string conversion is supported */
    if ( type == FUDGE_TYPE_STRING )
    {
        const ExampleIp4Struct * ip4 = ( const ExampleIp4Struct * ) source->data.bytes;
        char buffer [ 16 ];

        sprintf ( buffer, "%03d.%03d.%03d.%03d", ip4->quads [ 0 ], ip4->quads [ 1 ], ip4->quads [ 2 ], ip4->quads [ 3 ] );
        if ( ! ( target->bytes = ( fudge_byte * ) malloc ( sizeof ( buffer ) ) ) )
            return FUDGE_OUT_OF_MEMORY;
        memcpy ( ( fudge_byte * ) target->bytes, buffer, sizeof ( buffer ) );

        *numbytes = sizeof ( buffer );
        return FUDGE_OK;
    }
    return FUDGE_INVALID_TYPE_COERCION;
}

/* ExampleTick functions */

void ExampleTick_init ( ExampleTickStruct * tick, const char * ric, fudge_i32 riclen, fudge_f32 bid, fudge_f32 ask, fudge_i32 time )
{
    memcpy ( tick->ric, ric, riclen < sizeof ( ExampleTickStruct ) ? riclen : sizeof ( ExampleTickStruct ) );
    tick->bid = bid;
    tick->ask = ask;
    tick->time = time;
}

FudgeStatus FudgeMsg_addFieldExampleTick ( FudgeMsg message, const fudge_byte * name, fudge_i32 namelen, const ExampleTickStruct * tick )
{
    if ( ! ( message && tick ) )
        return FUDGE_NULL_POINTER;

    return FudgeMsg_addFieldOpaque ( message,
                                     FUDGE_TYPE_EXAMPLETICK,
                                     name,
                                     namelen,
                                     0,
                                     ( const fudge_byte * ) tick,
                                     sizeof ( ExampleTickStruct ) );
}

FudgeStatus FudgeCodec_encodeFieldExampleTick ( const FudgeField * field, fudge_byte * * data )
{
    const ExampleTickStruct * tick = ( const ExampleTickStruct * ) field->data.bytes;

    FudgeCodec_encodeFieldLength ( field->numbytes, data );

    /* Write the RIC - this is written as a fixed width field as that's what
       it is in the ExampleTickStruct structure. */
    FudgeCodec_encodeByteArray ( ( const fudge_byte * ) tick->ric, 16, FUDGE_TRUE, data );

    /* Write the float and integer values - this will perform any necessary
       endian conversions. */
    FudgeCodec_encodeF32 ( tick->bid, data );
    FudgeCodec_encodeF32 ( tick->ask, data );
    FudgeCodec_encodeI32 ( tick->time, data );
    return FUDGE_OK;
}

FudgeStatus FudgeCodec_decodeFieldExampleTick ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data )
{
    ExampleTickStruct * tick;

    /* Make sure that the width in the header matches that of the target structure */
    if ( width != sizeof ( ExampleTickStruct ) )
        return FUDGE_OUT_OF_BYTES;

    /* Allocate the target structure and point the data structure's bytes pointer at it */
    if ( ! ( tick = ( ExampleTickStruct * ) malloc ( width ) ) )
        return FUDGE_OUT_OF_MEMORY;
    data->bytes = ( const fudge_byte * ) tick;

    /* Copy in the string component and advance the source bytes pointer */
    memcpy ( tick->ric, bytes, 16 );
    bytes += 16;

    /* Copy in the float/integer values, advancing the bytes pointer after each one */
    tick->bid = FudgeCodec_decodeF32 ( bytes );     bytes += sizeof ( fudge_f32 );
    tick->ask = FudgeCodec_decodeF32 ( bytes );     bytes += sizeof ( fudge_f32 );
    tick->time = FudgeCodec_decodeI32 ( bytes );    bytes += sizeof ( fudge_f32 );

    return FUDGE_OK;
}

FudgeStatus FudgeType_coerceExampleTick ( const FudgeField * source, const fudge_type_id type, FudgeFieldData * target, fudge_i32 * numbytes )
{
    if ( ! ( source && target && numbytes ) )
        return FUDGE_NULL_POINTER;
    if ( source->type == type )
        return FUDGE_COERCION_NOT_REQUIRED;

    /* For test purposes, only a string conversion is supported */
    if ( type == FUDGE_TYPE_STRING )
    {
        const ExampleTickStruct * tick = ( const ExampleTickStruct * ) source->data.bytes;
        char buffer [ 52 ];

        sprintf ( buffer, "%16s %8f %8f %16d", tick->ric, tick->bid, tick->ask, tick->time );
        if ( ! ( target->bytes = ( fudge_byte * ) malloc ( sizeof ( buffer ) ) ) )
            return FUDGE_OUT_OF_MEMORY;
        memcpy ( ( fudge_byte * ) target->bytes, buffer, sizeof ( buffer ) );

        *numbytes = sizeof ( buffer );
        return FUDGE_OK;
    }
    return FUDGE_INVALID_TYPE_COERCION;
}

DEFINE_TEST( UserTypeHandling )
    int index;
    FudgeMsg message;
    FudgeMsgEnvelope envelope;
    FudgeField field;
    FudgeFieldData fielddata;
    FudgeTypePayload payload;
    fudge_byte * encoded;
    fudge_i32 encodedsize, fieldsize;
    ExampleIp4Struct localhost, opendns;
    ExampleTickStruct tick;
    fudge_i64 bigint;
    fudge_byte bytes [ 16 ];

    /* Populate test objects */
    for ( index = 0; index < sizeof ( bytes ); ++index )
        bytes [ index ] = ( fudge_byte ) index;
    bigint = 2147483648ul;
    ExampleIp4_init ( &localhost, 127, 0, 0, 1 );
    ExampleIp4_init ( &opendns, 208, 67, 222, 222 );
    ExampleTick_init ( &tick, "GBP=", 5, 1.605, 1.607, 1263138018u );

    /* Fail to register user types */
    TEST_EQUALS_INT( FudgeRegistry_registerType ( FUDGE_TYPE_EXAMPLEIP4,
                                                  FUDGE_TYPE_PAYLOAD_LOCAL,
                                                  FudgeCodec_decodeFieldByteArray,
                                                  FudgeCodec_encodeFieldByteArray,
                                                  FudgeType_coerceExampleIp4 ), FUDGE_INVALID_USER_TYPE );

    TEST_EQUALS_INT( FudgeRegistry_registerType ( FUDGE_TYPE_EXAMPLEIP4,
                                                  FUDGE_TYPE_PAYLOAD_BYTES,
                                                  FudgeCodec_decodeFieldByteArray,
                                                  0,
                                                  FudgeType_coerceExampleIp4 ), FUDGE_NULL_POINTER );

    /* Register user types */
    TEST_EQUALS_INT( FudgeRegistry_registerType ( FUDGE_TYPE_EXAMPLEIP4,
                                                  FUDGE_TYPE_PAYLOAD_BYTES,
                                                  FudgeCodec_decodeFieldByteArray,
                                                  FudgeCodec_encodeFieldByteArray,
                                                  FudgeType_coerceExampleIp4 ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeRegistry_registerType ( FUDGE_TYPE_EXAMPLETICK,
                                                  FUDGE_TYPE_PAYLOAD_BYTES,
                                                  FudgeCodec_decodeFieldExampleTick,
                                                  FudgeCodec_encodeFieldExampleTick,
                                                  FudgeType_coerceExampleTick ), FUDGE_OK );

    /* Construct a test message with user and built-in types */
    TEST_EQUALS_INT( FudgeMsg_create ( &message ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_addFieldI64 ( message, ( const fudge_byte * ) "bigint", 6, 0, bigint ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldExampleIp4 ( message, ( const fudge_byte * ) "localhost", 9, &localhost ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldExampleIp4 ( message, ( const fudge_byte * ) "opendns", 7, &opendns ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldExampleTick ( message, ( const fudge_byte * ) "tick", 4, &tick ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addField16ByteArray ( message, ( const fudge_byte * ) "byte[16]", 8, 0, bytes ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 5 );

    /* Encode the test message */
    envelope.directives = 0;
    envelope.schemaversion = 0;
    envelope.taxonomy = 0;
    envelope.message = message;

    TEST_EQUALS_INT( FudgeCodec_encodeMsg ( envelope, &encoded, &encodedsize ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );
    envelope.message = 0;

    /* Decode the test message */
    TEST_EQUALS_INT( FudgeCodec_decodeMsg ( &envelope, encoded, encodedsize ), FUDGE_OK );
    TEST_EQUALS_TRUE( envelope.message != 0 );
    message = envelope.message;
    free ( encoded );

    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 5 );

    /* Retrieve the decoded fields */
    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, message, ( const fudge_byte * ) "bigint", 6 ), FUDGE_OK );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_LONG );             TEST_EQUALS_INT( field.data.i64, bigint );
    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, message, ( const fudge_byte * ) "byte[16]", 8 ), FUDGE_OK );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_BYTE_ARRAY_16 );    TEST_EQUALS_MEMORY( field.data.bytes, field.numbytes, bytes, sizeof ( bytes ) );

    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, message, ( const fudge_byte * ) "localhost", 9 ), FUDGE_OK );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_EXAMPLEIP4 );       TEST_EQUALS_MEMORY( field.data.bytes, field.numbytes, ( fudge_byte * ) &localhost, sizeof ( ExampleIp4Struct ) );
    TEST_EQUALS_INT( FudgeMsg_getFieldAs ( &field, FUDGE_TYPE_EXAMPLEIP4, &fielddata, &payload, &fieldsize ), FUDGE_COERCION_NOT_REQUIRED );
    TEST_EQUALS_INT( FudgeMsg_getFieldAs ( &field, FUDGE_TYPE_FUDGE_MSG, &fielddata, &payload, &fieldsize ), FUDGE_INVALID_TYPE_COERCION );
    TEST_EQUALS_INT( FudgeMsg_getFieldAs ( &field, FUDGE_TYPE_STRING, &fielddata, &payload, &fieldsize ), FUDGE_OK );
    TEST_EQUALS_INT( payload, FUDGE_TYPE_PAYLOAD_BYTES );       TEST_EQUALS_MEMORY( fielddata.bytes, fieldsize, "127.000.000.001", 16 );
    free ( ( fudge_byte * ) fielddata.bytes );
    
    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, message, ( const fudge_byte * ) "opendns", 7 ), FUDGE_OK );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_EXAMPLEIP4 );       TEST_EQUALS_MEMORY( field.data.bytes, field.numbytes, ( fudge_byte * ) &opendns, sizeof ( ExampleIp4Struct ) );
    TEST_EQUALS_INT( FudgeMsg_getFieldAs ( &field, FUDGE_TYPE_STRING, &fielddata, &payload, &fieldsize ), FUDGE_OK );
    TEST_EQUALS_INT( payload, FUDGE_TYPE_PAYLOAD_BYTES );       TEST_EQUALS_MEMORY( fielddata.bytes, fieldsize, "208.067.222.222", 16 );
    free ( ( fudge_byte * ) fielddata.bytes );

    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, message, ( const fudge_byte * ) "tick", 4 ), FUDGE_OK );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_EXAMPLETICK );      TEST_EQUALS_MEMORY( field.data.bytes, field.numbytes, ( fudge_byte * ) &tick, sizeof ( ExampleTickStruct ) );
    TEST_EQUALS_INT( FudgeMsg_getFieldAs ( &field, FUDGE_TYPE_STRING, &fielddata, &payload, &fieldsize ), FUDGE_OK );
    TEST_EQUALS_INT( payload, FUDGE_TYPE_PAYLOAD_BYTES );       TEST_EQUALS_MEMORY( fielddata.bytes, fieldsize, "            GBP= 1.605000 1.607000       1263138018", 52 );
    free ( ( fudge_byte * ) fielddata.bytes );

    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );
END_TEST

DEFINE_TEST_SUITE( UserTypes )
    REGISTER_TEST( UserTypeHandling )
END_TEST_SUITE

