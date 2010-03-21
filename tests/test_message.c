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
#include "fudge/datetime.h"
#include "fudge/message.h"
#include "fudge/string.h"
#include "fudge/stringpool.h"
#include "simpletest.h"

DEFINE_TEST( FieldFunctions )
    static const fudge_byte rawBytes [ 16 ] = { 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00 };
    static const fudge_i16 rawShorts [ 10 ] = { -32767, 32767, 0, 1, -1, 100, -100, 0, 16385 };
    static const fudge_i32 rawInts [ 4 ] = { 2147483647, 0, -2147483647, 0 };
    static const fudge_i64 rawLongs [ 12 ] = { ( fudge_i64 ) -9223372036854775807ll, 0, ( fudge_i64 ) 9223372036854775807ll, -1, 2, -3, 5, -8, 13, -21, 34, -55 };
    static const fudge_f32 rawFloats [ 8 ] = { 0.0f, 2147483647.0f, 214748364.7f, 21474836.47f, 2147483.647f, 2.147483647f, 21.47483647f, 214.7483647f };
    static const fudge_f64 rawDoubles [ 5 ] = { 9223372036854775807.0, 0.0, 0.0000000123456, 1234560000000.0, -9223372036854775807.0 };

    fudge_byte largeByteArray [ 512 ];   /* Will need a large byte array for the fix array testing */
    
    FudgeMsg message, submessage;
    FudgeStatus status;
    FudgeStringPool stringpool;
    FudgeString string;
    FudgeDateTime datetime;
    FudgeField field;
    FudgeField fields [ 32 ];            /* Needs to be big enough to hold the largest field in the test */
    int index;
    fudge_i16 ordinal;

    TEST_EQUALS_INT( FudgeStringPool_create ( &stringpool ), FUDGE_OK );

    /* Populate the large byte array before use it in the tests */
    for ( index = 0; index < sizeof ( largeByteArray ); ++index )
        largeByteArray [ index ] = index % 256 - 128;

    /* Construct an empty message */
    TEST_EQUALS_INT( FudgeMsg_create ( &message ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 0 );

    /* Test some failure cases */
    TEST_EQUALS_INT( FudgeMsg_addFieldBool ( 0, 0, 0, FUDGE_FALSE ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeMsg_addFieldMsg ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Null message" ), 0, 0 ), FUDGE_NULL_POINTER );              TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldString ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Null string pointer" ), 0, 0 ), FUDGE_NULL_POINTER );    TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 0 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( 0, message, 0 ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( fields, message, 0 ), FUDGE_INVALID_INDEX );
    TEST_EQUALS_INT( FudgeMsg_addFieldBool ( message,
                                             FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "This message field name is too long. The limit for message field names is 256 characters; this is because "
                                                                                                           "the name's length must be stored in a single leading byte (a bit like a length limited version Pascal). A "
                                                                                                           "special error code exists just for this eventuality, so hopefully it will be thrown here." ),
                                             0,
                                             FUDGE_TRUE ), FUDGE_NAME_TOO_LONG );
    TEST_EQUALS_INT( status, FUDGE_OK );

    /* Add indicator, boolean, integer and float fields */
    TEST_EQUALS_INT( FudgeMsg_addFieldIndicator ( message, 0, 0 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldBool ( message, 0, 0, FUDGE_TRUE ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldByte ( message, 0, 0, 127 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI16 ( message, 0, 0, ( fudge_i16 ) 32767 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI32 ( message, 0, 0, ( fudge_i32 ) -2147483647 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI64 ( message, 0, 0, ( fudge_i64 ) 9223372036854775807ll ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF32 ( message, 0, 0, 2147483647.0f ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF64 ( message, 0, 0, -9223372036854775807.0 ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 8 );

    /* Add date, time and date/time */
    TEST_EQUALS_INT( FudgeDate_initialise ( &( datetime.date ), 2010, 3, 21 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeTime_initialise ( &( datetime.time ), 15 * 3600 + 39 * 60 + 27, 0, FUDGE_DATETIME_PRECISION_SECOND ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldDate ( message, 0, 0, &( datetime.date ) ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldTime ( message, 0, 0, &( datetime.time ) ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldDateTime ( message, 0, 0, &datetime ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 11 );

    /* Add fixed width byte arrays in a sub message */
    TEST_EQUALS_INT( FudgeMsg_create ( &submessage ), FUDGE_OK );
    ordinal = 1; TEST_EQUALS_INT( FudgeMsg_addField4ByteArray ( submessage, 0, &ordinal, largeByteArray ), FUDGE_OK );
    ordinal = 2; TEST_EQUALS_INT( FudgeMsg_addField8ByteArray ( submessage, 0, &ordinal, largeByteArray ), FUDGE_OK );
    ordinal = 3; TEST_EQUALS_INT( FudgeMsg_addField16ByteArray ( submessage, 0, &ordinal, largeByteArray ), FUDGE_OK );
    ordinal = 4; TEST_EQUALS_INT( FudgeMsg_addField20ByteArray ( submessage, 0, &ordinal, largeByteArray ), FUDGE_OK );
    ordinal = 5; TEST_EQUALS_INT( FudgeMsg_addField32ByteArray ( submessage, 0, &ordinal, largeByteArray ), FUDGE_OK );
    ordinal = 6; TEST_EQUALS_INT( FudgeMsg_addField64ByteArray ( submessage, 0, &ordinal, largeByteArray ), FUDGE_OK );
    ordinal = 7; TEST_EQUALS_INT( FudgeMsg_addField128ByteArray ( submessage, 0, &ordinal, largeByteArray ), FUDGE_OK );
    ordinal = 8; TEST_EQUALS_INT( FudgeMsg_addField256ByteArray ( submessage, 0, &ordinal, largeByteArray ), FUDGE_OK );
    ordinal = 9; TEST_EQUALS_INT( FudgeMsg_addField512ByteArray ( submessage, 0, &ordinal, largeByteArray ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_numFields ( submessage ), 9 );

    TEST_EQUALS_INT( FudgeMsg_addFieldMsg ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Byte Array SubMessage" ), 0, submessage ), FUDGE_OK );  TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( submessage ), FUDGE_OK );

    /* Add null, empty and populated strings */
    TEST_EQUALS_INT( FudgeString_createFromASCIIZ ( &string, 0 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldString ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Null string" ), 0, string ), FUDGE_OK );     TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeString_release ( string ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeString_createFromASCIIZ ( &string, "" ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldString ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Empty string" ), 0, string ), FUDGE_OK );    TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeString_release ( string ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeString_createFromASCIIZ ( &string, "This is a string" ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldString ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "String" ), 0, string ), FUDGE_OK );          TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeString_release ( string ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 15 );

    /* Add empty and populated arrays in a sub message */
    TEST_EQUALS_INT( FudgeMsg_create ( &submessage ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_addFieldByteArray ( submessage, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "No Bytes" ), 0, 0, 0 ), FUDGE_OK );        TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldByteArray ( submessage, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Bytes" ), 0, rawBytes, 16 ), FUDGE_OK );   TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI16Array ( submessage, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Shorts" ), 0, rawShorts, 10 ), FUDGE_OK );  TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI32Array ( submessage, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Ints" ), 0, rawInts, 4 ), FUDGE_OK );       TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI64Array ( submessage, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Longs" ), 0, rawLongs, 12 ), FUDGE_OK );    TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF32Array ( submessage, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Floats" ), 0, rawFloats, 8 ), FUDGE_OK );   TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF64Array ( submessage, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Doubles" ), 0, rawDoubles, 5 ), FUDGE_OK ); TEST_EQUALS_INT( status, FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_numFields ( submessage ), 7 );

    ordinal = 0;
    TEST_EQUALS_INT( FudgeMsg_addFieldMsg ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Array SubMessage" ), &ordinal, submessage ), FUDGE_OK );
    TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( submessage ), FUDGE_OK );

    /* Add an empty sub message */
    TEST_EQUALS_INT( FudgeMsg_create ( &submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldMsg ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Empty SubMessage" ), 0, submessage ), FUDGE_OK );
    TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( submessage ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 17 );

    /* Retrieve the top-level fields and check their contents */
    for ( index = 0; index < FudgeMsg_numFields ( message ); ++index )
        TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( fields + index, message, index ), FUDGE_OK );

    TEST_EQUALS_INT( fields [ 0 ].type, FUDGE_TYPE_INDICATOR );
    TEST_EQUALS_INT( fields [ 1 ].type, FUDGE_TYPE_BOOLEAN );
    TEST_EQUALS_TRUE( fields [ 1 ].data.boolean );
    TEST_EQUALS_INT( fields [ 2 ].type, FUDGE_TYPE_BYTE );
    TEST_EQUALS_INT( fields [ 2 ].data.byte, 127 );
    TEST_EQUALS_INT( fields [ 3 ].type, FUDGE_TYPE_SHORT );
    TEST_EQUALS_INT( fields [ 3 ].data.i16, 32767 );
    TEST_EQUALS_INT( fields [ 4 ].type, FUDGE_TYPE_INT );
    TEST_EQUALS_INT( fields [ 4 ].data.i32, -2147483647 );
    TEST_EQUALS_INT( fields [ 5 ].type, FUDGE_TYPE_LONG );
    TEST_EQUALS_INT( fields [ 5 ].data.i64, 9223372036854775807ll );
    TEST_EQUALS_INT( fields [ 6 ].type, FUDGE_TYPE_FLOAT );
    TEST_EQUALS_FLOAT( fields [ 6 ].data.f32, 2147483647.0f, 0.01f );
    TEST_EQUALS_INT( fields [ 7 ].type, FUDGE_TYPE_DOUBLE );
    TEST_EQUALS_FLOAT( fields [ 7 ].data.f64, -9223372036854775807.0, 0.01f );
    TEST_EQUALS_INT( fields [ 8 ].type, FUDGE_TYPE_DATE );
    TEST_EQUALS_DATE( fields [ 8 ].data.datetime.date, 2010, 3, 21 );
    TEST_EQUALS_INT( fields [ 9 ].type, FUDGE_TYPE_TIME );
    TEST_EQUALS_TIME( fields [ 9 ].data.datetime.time, 15 * 3600 + 39 * 60 + 27, 0, FUDGE_DATETIME_PRECISION_SECOND, -128 );
    TEST_EQUALS_INT( fields [ 10 ].type, FUDGE_TYPE_DATETIME );
    TEST_EQUALS_DATE( fields [ 10 ].data.datetime.date, 2010, 3, 21 );
    TEST_EQUALS_TIME( fields [ 10 ].data.datetime.time, 15 * 3600 + 39 * 60 + 27, 0, FUDGE_DATETIME_PRECISION_SECOND, -128 );
    TEST_EQUALS_INT( fields [ 11 ].type, FUDGE_TYPE_FUDGE_MSG );
    TEST_EQUALS_TRUE( fields [ 11 ].data.message != 0 );
    TEST_EQUALS_INT( fields [ 12 ].type, FUDGE_TYPE_STRING );
    TEST_EQUALS_MEMORY( FudgeString_getData ( fields [ 12 ].data.string ), FudgeString_getSize ( fields [ 12 ].data.string ), 0, 0 );
    TEST_EQUALS_INT( fields [ 13 ].type, FUDGE_TYPE_STRING );
    TEST_EQUALS_MEMORY( FudgeString_getData ( fields [ 13 ].data.string ), FudgeString_getSize ( fields [ 13 ].data.string ), 0, 0 );
    TEST_EQUALS_INT( fields [ 14 ].type, FUDGE_TYPE_STRING );
    TEST_EQUALS_MEMORY( FudgeString_getData ( fields [ 14 ].data.string ), FudgeString_getSize ( fields [ 14 ].data.string ), "This is a string", 16 );
    TEST_EQUALS_INT( fields [ 15 ].type, FUDGE_TYPE_FUDGE_MSG );
    TEST_EQUALS_TRUE( fields [ 15 ].data.message != 0 );
    TEST_EQUALS_INT( fields [ 16 ].type, FUDGE_TYPE_FUDGE_MSG );
    TEST_EQUALS_TRUE( fields [ 16 ].data.message != 0 );

    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Null string" ) ), FUDGE_OK );
    TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_STRING );
    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Empty SubMessage" ) ), FUDGE_OK );
    TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_FUDGE_MSG );

    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "" ) ),            FUDGE_INVALID_NAME );  TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "null string" ) ), FUDGE_INVALID_NAME );  TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Bytes" ) ),       FUDGE_INVALID_NAME );  TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_getFieldByOrdinal ( &field, message, 10 ), FUDGE_INVALID_ORDINAL );

    /* Check the fixed array message */
    submessage = fields [ 11 ].data.message;
    TEST_EQUALS_INT( FudgeMsg_numFields ( submessage ), 9 );
    for ( index = 0; index < FudgeMsg_numFields ( submessage ); ++index )
        TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( fields + index, submessage, index ), FUDGE_OK );
    
    TEST_EQUALS_INT( fields [ 0 ].type, FUDGE_TYPE_BYTE_ARRAY_4 );
    TEST_EQUALS_INT( fields [ 0 ].numbytes, 4 );
    TEST_EQUALS_MEMORY( fields [ 0 ].data.bytes, 4, largeByteArray, 4 );
    TEST_EQUALS_INT( fields [ 1 ].type, FUDGE_TYPE_BYTE_ARRAY_8 );
    TEST_EQUALS_INT( fields [ 1 ].numbytes, 8 );
    TEST_EQUALS_MEMORY( fields [ 1 ].data.bytes, 8, largeByteArray, 8 );
    TEST_EQUALS_INT( fields [ 2 ].type, FUDGE_TYPE_BYTE_ARRAY_16 );
    TEST_EQUALS_INT( fields [ 2 ].numbytes, 16 );
    TEST_EQUALS_MEMORY( fields [ 2 ].data.bytes, 16, largeByteArray, 16 );
    TEST_EQUALS_INT( fields [ 3 ].type, FUDGE_TYPE_BYTE_ARRAY_20 );
    TEST_EQUALS_INT( fields [ 3 ].numbytes, 20 );
    TEST_EQUALS_MEMORY( fields [ 3 ].data.bytes, 20, largeByteArray, 20 );
    TEST_EQUALS_INT( fields [ 4 ].type, FUDGE_TYPE_BYTE_ARRAY_32 );
    TEST_EQUALS_INT( fields [ 4 ].numbytes, 32 );
    TEST_EQUALS_MEMORY( fields [ 4 ].data.bytes, 32, largeByteArray, 32 );
    TEST_EQUALS_INT( fields [ 5 ].type, FUDGE_TYPE_BYTE_ARRAY_64 );
    TEST_EQUALS_INT( fields [ 5 ].numbytes, 64 );
    TEST_EQUALS_MEMORY( fields [ 5 ].data.bytes, 64, largeByteArray, 64 );
    TEST_EQUALS_INT( fields [ 6 ].type, FUDGE_TYPE_BYTE_ARRAY_128 );
    TEST_EQUALS_INT( fields [ 6 ].numbytes, 128 );
    TEST_EQUALS_MEMORY( fields [ 6 ].data.bytes, 128, largeByteArray, 128 );
    TEST_EQUALS_INT( fields [ 7 ].type, FUDGE_TYPE_BYTE_ARRAY_256 );
    TEST_EQUALS_INT( fields [ 7 ].numbytes, 256 );
    TEST_EQUALS_MEMORY( fields [ 7 ].data.bytes, 256, largeByteArray, 256 );
    TEST_EQUALS_INT( fields [ 8 ].type, FUDGE_TYPE_BYTE_ARRAY_512 );
    TEST_EQUALS_INT( fields [ 8 ].numbytes, 512 );
    TEST_EQUALS_MEMORY( fields [ 8 ].data.bytes, 512, largeByteArray, 512 );

    TEST_EQUALS_INT( FudgeMsg_getFieldByOrdinal ( &field, submessage, 1 ), FUDGE_OK );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_BYTE_ARRAY_4 );
    TEST_EQUALS_INT( FudgeMsg_getFieldByOrdinal ( &field, submessage, 4 ), FUDGE_OK );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_BYTE_ARRAY_20 );

    TEST_EQUALS_INT( FudgeMsg_getFieldByOrdinal ( &field, submessage, 0 ), FUDGE_INVALID_ORDINAL );
    TEST_EQUALS_INT( FudgeMsg_getFieldByOrdinal ( &field, submessage, -1 ), FUDGE_INVALID_ORDINAL );
    TEST_EQUALS_INT( FudgeMsg_getFieldByOrdinal ( &field, submessage, 10 ), FUDGE_INVALID_ORDINAL );

    /* Check the variable array message */
    TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( &field, message, 15 ), FUDGE_OK );
    submessage = field.data.message;
    TEST_EQUALS_INT( FudgeMsg_numFields ( submessage ), 7 );
    TEST_EQUALS_INT( FudgeMsg_getFields ( fields, sizeof ( fields ) / sizeof ( FudgeField ), submessage ), 7 );

    TEST_EQUALS_INT( fields [ 0 ].type, FUDGE_TYPE_BYTE_ARRAY );
    TEST_EQUALS_INT( fields [ 0 ].numbytes, 0 );
    TEST_EQUALS_MEMORY( fields [ 0 ].data.bytes, 0, 0, 0 );
    TEST_EQUALS_INT( fields [ 1 ].type, FUDGE_TYPE_BYTE_ARRAY );
    TEST_EQUALS_INT( fields [ 1 ].numbytes, 16 );
    TEST_EQUALS_MEMORY( fields [ 1 ].data.bytes, 16, rawBytes, 16 );
    TEST_EQUALS_INT( fields [ 2 ].type, FUDGE_TYPE_SHORT_ARRAY );
    TEST_EQUALS_INT( fields [ 2 ].numbytes, 20 );
    TEST_EQUALS_MEMORY( fields [ 2 ].data.bytes, 20, rawShorts, 20 );
    TEST_EQUALS_INT( fields [ 3 ].type, FUDGE_TYPE_INT_ARRAY );
    TEST_EQUALS_INT( fields [ 3 ].numbytes, 16 );
    TEST_EQUALS_MEMORY( fields [ 3 ].data.bytes, 16, rawInts, 16 );
    TEST_EQUALS_INT( fields [ 4 ].type, FUDGE_TYPE_LONG_ARRAY );
    TEST_EQUALS_INT( fields [ 4 ].numbytes, 96 );
    TEST_EQUALS_MEMORY( fields [ 4 ].data.bytes, 96, rawLongs, 96 );
    TEST_EQUALS_INT( fields [ 5 ].type, FUDGE_TYPE_FLOAT_ARRAY );
    TEST_EQUALS_INT( fields [ 5 ].numbytes, 32 );
    TEST_EQUALS_MEMORY( fields [ 5 ].data.bytes, 32, rawFloats, 32 );
    TEST_EQUALS_INT( fields [ 6 ].type, FUDGE_TYPE_DOUBLE_ARRAY );
    TEST_EQUALS_INT( fields [ 6 ].numbytes, 40 );
    TEST_EQUALS_MEMORY( fields [ 6 ].data.bytes, 40, rawDoubles, 40 );

    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, submessage, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Bytes" ) ), FUDGE_OK );
    TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_BYTE_ARRAY );
    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, submessage, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Floats" ) ), FUDGE_OK );
    TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_FLOAT_ARRAY );

    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, submessage, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Empty SubMessage" ) ), FUDGE_INVALID_NAME );
    TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_getFieldByOrdinal ( &field, submessage, 1 ), FUDGE_INVALID_ORDINAL );

    /* Check the empty message */
    TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( &field, message, 16 ), FUDGE_OK );
    submessage = field.data.message;
    TEST_EQUALS_INT( FudgeMsg_numFields ( submessage ), 0 );

    /* Clean up */
    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeStringPool_release ( stringpool ), FUDGE_OK );
END_TEST

DEFINE_TEST( IntegerFieldDowncasting )
    FudgeMsg message;
    FudgeField fields [ 32 ];

    TEST_EQUALS_INT( FudgeMsg_create ( &message ), FUDGE_OK );

    /* Add the test fields */
    TEST_EQUALS_INT( FudgeMsg_addFieldByte ( message, 0, 0, -127 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI16  ( message, 0, 0, -127 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI16  ( message, 0, 0, 32767 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI32  ( message, 0, 0, -127 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI32  ( message, 0, 0, 32767 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI32  ( message, 0, 0, -2147483647 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI64  ( message, 0, 0, -127 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI64  ( message, 0, 0, 32767 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI64  ( message, 0, 0, -2147483647 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI64  ( message, 0, 0, ( fudge_i64 ) 2147483648ul ), FUDGE_OK );

    /* Check the test fields */
    TEST_EQUALS_INT( FudgeMsg_getFields ( fields, sizeof ( fields ) / sizeof ( FudgeField ), message ), 10 );

    TEST_EQUALS_INT( fields [ 0 ].type, FUDGE_TYPE_BYTE );      TEST_EQUALS_INT( fields [ 0 ].data.byte, -127 );
    TEST_EQUALS_INT( fields [ 1 ].type, FUDGE_TYPE_BYTE );      TEST_EQUALS_INT( fields [ 1 ].data.byte, -127 );
    TEST_EQUALS_INT( fields [ 2 ].type, FUDGE_TYPE_SHORT );     TEST_EQUALS_INT( fields [ 2 ].data.i16, 32767 );
    TEST_EQUALS_INT( fields [ 3 ].type, FUDGE_TYPE_BYTE );      TEST_EQUALS_INT( fields [ 3 ].data.byte, -127 );
    TEST_EQUALS_INT( fields [ 4 ].type, FUDGE_TYPE_SHORT );     TEST_EQUALS_INT( fields [ 4 ].data.i16, 32767 );
    TEST_EQUALS_INT( fields [ 5 ].type, FUDGE_TYPE_INT );       TEST_EQUALS_INT( fields [ 5 ].data.i32, -2147483647 );
    TEST_EQUALS_INT( fields [ 6 ].type, FUDGE_TYPE_BYTE );      TEST_EQUALS_INT( fields [ 6 ].data.byte, -127 );
    TEST_EQUALS_INT( fields [ 7 ].type, FUDGE_TYPE_SHORT );     TEST_EQUALS_INT( fields [ 7 ].data.i16, 32767 );
    TEST_EQUALS_INT( fields [ 8 ].type, FUDGE_TYPE_INT );       TEST_EQUALS_INT( fields [ 8 ].data.i32, -2147483647 );
    TEST_EQUALS_INT( fields [ 9 ].type, FUDGE_TYPE_LONG );      TEST_EQUALS_INT( fields [ 9 ].data.i64, ( fudge_i64 ) 2147483648ul );

    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );
END_TEST

DEFINE_TEST( FieldCoercion )
    FudgeMsg message;
    FudgeField fields [ 32 ];
    fudge_bool boolean;
    fudge_byte byte;
    fudge_i16 i16;
    fudge_i32 i32, index, numfields;
    fudge_i64 i64;
    fudge_f32 f32;
    fudge_f64 f64;
    FudgeStringPool stringpool;
    FudgeStatus status;

    TEST_EQUALS_INT( FudgeMsg_create ( &message ), FUDGE_OK );

    /* Add the test fields */
    TEST_EQUALS_INT( FudgeStringPool_create ( &stringpool ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldIndicator ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Indicator" ), 0 ), FUDGE_OK );                TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldBool ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "True Bool" ), 0, FUDGE_TRUE ), FUDGE_OK );         TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldBool ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "False Bool" ), 0, FUDGE_FALSE ), FUDGE_OK );       TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldByte ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Zero Byte" ), 0, 0 ), FUDGE_OK );                  TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldByte ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Non-zero Byte" ), 0, -42 ), FUDGE_OK );            TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI16 ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Non-zero Short" ), 0, 256 ), FUDGE_OK );            TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI32 ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Non-zero Int" ), 0, -40000 ), FUDGE_OK );           TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI64 ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Non-zero Long" ), 0, 10000000000ll ), FUDGE_OK );   TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF32 ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Zero Float" ), 0, 0.0 ), FUDGE_OK );                TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF32 ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Non-zero Float" ), 0, -1.234f ), FUDGE_OK );        TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF64 ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Zero Double" ), 0, 0.0 ), FUDGE_OK );               TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF64 ( message, FudgeStringPool_createStringFromASCIIZ ( stringpool, &status, "Non-zero Double" ), 0, 123.4567 ), FUDGE_OK );      TEST_EQUALS_INT( status, FUDGE_OK );
    TEST_EQUALS_INT( FudgeStringPool_release ( stringpool ), FUDGE_OK );

    /* Retrieve test fields and test casting */
    numfields = FudgeMsg_numFields ( message );
    TEST_EQUALS_INT( FudgeMsg_getFields ( fields, sizeof ( fields ) / sizeof ( FudgeField ), message ), numfields );

    TEST_EQUALS_INT( FudgeMsg_getFieldAsBoolean ( fields, &boolean ), FUDGE_INVALID_TYPE_COERCION );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsBoolean ( fields + 1, &boolean ), FUDGE_OK );   TEST_EQUALS_INT( boolean, FUDGE_TRUE );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsBoolean ( fields + 2, &boolean ), FUDGE_OK );   TEST_EQUALS_INT( boolean, FUDGE_FALSE );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsBoolean ( fields + 3, &boolean ), FUDGE_OK );   TEST_EQUALS_INT( boolean, FUDGE_FALSE );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsBoolean ( fields + 4, &boolean ), FUDGE_OK );   TEST_EQUALS_INT( boolean, FUDGE_TRUE);
    TEST_EQUALS_INT( FudgeMsg_getFieldAsBoolean ( fields + 5, &boolean ), FUDGE_OK );   TEST_EQUALS_INT( boolean, FUDGE_TRUE);
    TEST_EQUALS_INT( FudgeMsg_getFieldAsBoolean ( fields + 6, &boolean ), FUDGE_OK );   TEST_EQUALS_INT( boolean, FUDGE_TRUE);
    TEST_EQUALS_INT( FudgeMsg_getFieldAsBoolean ( fields + 7, &boolean ), FUDGE_OK );   TEST_EQUALS_INT( boolean, FUDGE_TRUE);
    for ( index = 8; index < numfields; ++index )
        TEST_EQUALS_INT( FudgeMsg_getFieldAsBoolean ( fields + index, &boolean ), FUDGE_INVALID_TYPE_COERCION );
    
    TEST_EQUALS_INT( FudgeMsg_getFieldAsByte ( fields, &byte ), FUDGE_INVALID_TYPE_COERCION );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsByte ( fields + 1,  &byte ), FUDGE_OK );  TEST_EQUALS_INT( byte, FUDGE_TRUE );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsByte ( fields + 2,  &byte ), FUDGE_OK );  TEST_EQUALS_INT( byte, FUDGE_FALSE );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsByte ( fields + 3,  &byte ), FUDGE_OK );  TEST_EQUALS_INT( byte, 0 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsByte ( fields + 4,  &byte ), FUDGE_OK );  TEST_EQUALS_INT( byte, -42 );
    for ( index = 5; index < numfields; ++index )
        TEST_EQUALS_INT( FudgeMsg_getFieldAsByte ( fields + index, &byte ), FUDGE_INVALID_TYPE_COERCION );

    TEST_EQUALS_INT( FudgeMsg_getFieldAsI16 ( fields, &i16 ), FUDGE_INVALID_TYPE_COERCION );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsI16 ( fields + 1,  &i16 ), FUDGE_OK );  TEST_EQUALS_INT( i16, FUDGE_TRUE );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsI16 ( fields + 2,  &i16 ), FUDGE_OK );  TEST_EQUALS_INT( i16, FUDGE_FALSE );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsI16 ( fields + 3,  &i16 ), FUDGE_OK );  TEST_EQUALS_INT( i16, 0 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsI16 ( fields + 4,  &i16 ), FUDGE_OK );  TEST_EQUALS_INT( i16, -42 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsI16 ( fields + 5,  &i16 ), FUDGE_OK );  TEST_EQUALS_INT( i16, 256 );
    for ( index = 6; index < numfields; ++index )
        TEST_EQUALS_INT( FudgeMsg_getFieldAsI16 ( fields + index, &i16 ), FUDGE_INVALID_TYPE_COERCION );

    TEST_EQUALS_INT( FudgeMsg_getFieldAsI32 ( fields, &i32 ), FUDGE_INVALID_TYPE_COERCION );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsI32 ( fields + 1,  &i32 ), FUDGE_OK );  TEST_EQUALS_INT( i32, FUDGE_TRUE );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsI32 ( fields + 2,  &i32 ), FUDGE_OK );  TEST_EQUALS_INT( i32, FUDGE_FALSE );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsI32 ( fields + 3,  &i32 ), FUDGE_OK );  TEST_EQUALS_INT( i32, 0 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsI32 ( fields + 4,  &i32 ), FUDGE_OK );  TEST_EQUALS_INT( i32, -42 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsI32 ( fields + 5,  &i32 ), FUDGE_OK );  TEST_EQUALS_INT( i32, 256 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsI32 ( fields + 6,  &i32 ), FUDGE_OK );  TEST_EQUALS_INT( i32, -40000 );
    for ( index = 7; index < numfields; ++index )
        TEST_EQUALS_INT( FudgeMsg_getFieldAsI32 ( fields + index, &i32 ), FUDGE_INVALID_TYPE_COERCION );

    TEST_EQUALS_INT( FudgeMsg_getFieldAsI64 ( fields, &i64 ), FUDGE_INVALID_TYPE_COERCION );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsI64 ( fields + 1,  &i64 ), FUDGE_OK );  TEST_EQUALS_INT( i64, FUDGE_TRUE );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsI64 ( fields + 2,  &i64 ), FUDGE_OK );  TEST_EQUALS_INT( i64, FUDGE_FALSE );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsI64 ( fields + 3,  &i64 ), FUDGE_OK );  TEST_EQUALS_INT( i64, 0 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsI64 ( fields + 4,  &i64 ), FUDGE_OK );  TEST_EQUALS_INT( i64, -42 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsI64 ( fields + 5,  &i64 ), FUDGE_OK );  TEST_EQUALS_INT( i64, 256 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsI64 ( fields + 6,  &i64 ), FUDGE_OK );  TEST_EQUALS_INT( i64, -40000 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsI64 ( fields + 7,  &i64 ), FUDGE_OK );  TEST_EQUALS_INT( i64, 10000000000ll );
    for ( index = 8; index < numfields; ++index )
        TEST_EQUALS_INT( FudgeMsg_getFieldAsI64 ( fields + index, &i64 ), FUDGE_INVALID_TYPE_COERCION );

    for ( index = 0; index < 8; ++index )
        TEST_EQUALS_INT( FudgeMsg_getFieldAsF32 ( fields + index, &f32 ), FUDGE_INVALID_TYPE_COERCION );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsF32 ( fields +  8, &f32 ), FUDGE_OK );   TEST_EQUALS_FLOAT( f32, 0.0, 0.001 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsF32 ( fields +  9, &f32 ), FUDGE_OK );   TEST_EQUALS_FLOAT( f32, -1.234, 0.001 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsF32 ( fields + 10, &f32 ), FUDGE_OK );   TEST_EQUALS_FLOAT( f32, 0.0, 0.001 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsF32 ( fields + 11, &f32 ), FUDGE_OK );   TEST_EQUALS_FLOAT( f32, 123.4567, 0.0001 );
    for ( index = 12; index < numfields; ++index )
        TEST_EQUALS_INT( FudgeMsg_getFieldAsF32 ( fields + index, &f32 ), FUDGE_INVALID_TYPE_COERCION );

    for ( index = 0; index < 8; ++index )
        TEST_EQUALS_INT( FudgeMsg_getFieldAsF64 ( fields + index, &f64 ), FUDGE_INVALID_TYPE_COERCION );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsF64 ( fields +  8, &f64 ), FUDGE_OK );   TEST_EQUALS_FLOAT( f64, 0.0, 0.001 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsF64 ( fields +  9, &f64 ), FUDGE_OK );   TEST_EQUALS_FLOAT( f64, -1.234, 0.001 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsF64 ( fields + 10, &f64 ), FUDGE_OK );   TEST_EQUALS_FLOAT( f64, 0.0, 0.001 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAsF64 ( fields + 11, &f64 ), FUDGE_OK );   TEST_EQUALS_FLOAT( f64, 123.4567, 0.0001 );
    for ( index = 12; index < numfields; ++index )
        TEST_EQUALS_INT( FudgeMsg_getFieldAsF64 ( fields + index, &f64 ), FUDGE_INVALID_TYPE_COERCION );

    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );
END_TEST

DEFINE_TEST_SUITE( Message )
    REGISTER_TEST( FieldFunctions )
    REGISTER_TEST( IntegerFieldDowncasting )
    REGISTER_TEST( FieldCoercion )
END_TEST_SUITE

