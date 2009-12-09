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
#include "fudge/message.h"
#include "simpletest.h"

DEFINE_TEST( FieldFunctions )
    static const fudge_byte rawBytes [ 16 ] = { 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00 };
    static const fudge_i16 rawShorts [ 10 ] = { -32767, 32767, 0, 1, -1, 100, -100, 0, 16385 };
    static const fudge_i32 rawInts [ 4 ] = { 2147483647, 0, -2147483647, 0 };
    static const fudge_i64 rawLongs [ 12 ] = { ( fudge_i64 ) -9223372036854775807ll, 0, ( fudge_i64 ) 9223372036854775807ll, -1, 2, -3, 5, -8, 13, -21, 34, -55 };
    static const fudge_f32 rawFloats [ 8 ] = { 0.0f, 2147483647.0f, 214748364.7f, 21474836.47f, 2147483.647, 2.147483647f, 21.47483647f, 214.7483647f };
    static const fudge_f64 rawDoubles [ 5 ] = { 9223372036854775807.0, 0.0, 0.0000000123456, 1234560000000.0, -9223372036854775807.0 };

    fudge_byte largeByteArray [ 512 ];   /* Will need a large byte array for the fix array testing */
    
    FudgeMsg message, submessage;
    FudgeField field;
    FudgeField fields [ 32 ];            /* Needs to be big enough to hold the largest field in the test */
    int index;
    fudge_i16 ordinal;

    /* Populate the large byte array before use it in the tests */
    for ( index = 0; index < sizeof ( largeByteArray ); ++index )
        largeByteArray [ index ] = index % 256 - 128;

    /* Construct an empty message */
    TEST_EQUALS_INT( FudgeMsg_create ( &message ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 0 );

    /* Test some failure cases */
    TEST_EQUALS_INT( FudgeMsg_addFieldBool ( 0, 0, 0, FUDGE_FALSE ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeMsg_addFieldMsg ( message, "Null message", 0, 0 ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeMsg_addFieldString ( message, "Null, non zero-length string", 0, 0, 1 ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 0 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( 0, message, 0 ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( fields, message, 0 ), FUDGE_INVALID_INDEX );
    TEST_EQUALS_INT( FudgeMsg_addFieldBool ( message, "This message field name is too long. The limit for message field names is 256 characters; this is because "
                                                      "the name's length must be stored in a single leading byte (a bit like a length limited version Pascal). A "
                                                      "special error code exists just for this eventuality, so hopefully it will be thrown here.", 0, FUDGE_TRUE ), FUDGE_NAME_TOO_LONG );

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

    TEST_EQUALS_INT( FudgeMsg_addFieldMsg ( message, "Byte Array SubMessage", 0, submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( submessage ), FUDGE_OK );

    /* Add null, empty and populated strings */
    TEST_EQUALS_INT( FudgeMsg_addFieldString ( message, "Null string", 0, 0, 0 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldString ( message, "Empty string", 0, "", 0 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldString ( message, "String", 0, "This is a string", 16 ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 12 );

    /* Add empty and populated arrays in a sub message */
    TEST_EQUALS_INT( FudgeMsg_create ( &submessage ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_addFieldByteArray ( submessage, "No Bytes", 0, 0, 0 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldByteArray ( submessage, "Bytes", 0, rawBytes, 16 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI16Array ( submessage, "Shorts", 0, rawShorts, 10 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI32Array ( submessage, "Ints", 0, rawInts, 4 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI64Array ( submessage, "Longs", 0, rawLongs, 12 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF32Array ( submessage, "Floats", 0, rawFloats, 8 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF64Array ( submessage, "Doubles", 0, rawDoubles, 5 ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_numFields ( submessage ), 7 );

    ordinal = 0; TEST_EQUALS_INT( FudgeMsg_addFieldMsg ( message, "Array SubMessage", &ordinal, submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( submessage ), FUDGE_OK );

    /* Add an empty sub message */
    TEST_EQUALS_INT( FudgeMsg_create ( &submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldMsg ( message, "Empty SubMessage", 0, submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( submessage ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 14 );

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
    TEST_EQUALS_INT( fields [ 8 ].type, FUDGE_TYPE_FUDGE_MSG );
    TEST_EQUALS_TRUE( fields [ 8 ].data.message != 0 );
    TEST_EQUALS_INT( fields [ 9 ].type, FUDGE_TYPE_STRING );
    TEST_EQUALS_MEMORY( fields [ 9 ].data.bytes, 0, 0, 0 );
    TEST_EQUALS_INT( fields [ 10 ].type, FUDGE_TYPE_STRING );
    TEST_EQUALS_MEMORY( fields [ 10 ].data.bytes, 0, 0, 0 );
    TEST_EQUALS_INT( fields [ 11 ].type, FUDGE_TYPE_STRING );
    TEST_EQUALS_MEMORY( fields [ 11 ].data.bytes, 16, "This is a string", 16 );
    TEST_EQUALS_INT( fields [ 12 ].type, FUDGE_TYPE_FUDGE_MSG );
    TEST_EQUALS_TRUE( fields [ 12 ].data.message != 0 );
    TEST_EQUALS_INT( fields [ 13 ].type, FUDGE_TYPE_FUDGE_MSG );
    TEST_EQUALS_TRUE( fields [ 13 ].data.message != 0 );

    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, message, "Null string" ), FUDGE_OK );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_STRING );
    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, message, "Empty SubMessage" ), FUDGE_OK );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_FUDGE_MSG );

    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, message, "" ), FUDGE_INVALID_NAME );
    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, message, "null string" ), FUDGE_INVALID_NAME );
    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, message, "Bytes" ), FUDGE_INVALID_NAME );
    TEST_EQUALS_INT( FudgeMsg_getFieldByOrdinal ( &field, message, 10 ), FUDGE_INVALID_ORDINAL );

    /* Check the fixed array message */
    submessage = fields [ 8 ].data.message;
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
    TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( &field, message, 12 ), FUDGE_OK );
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

    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, submessage, "Bytes" ), FUDGE_OK );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_BYTE_ARRAY );
    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, submessage, "Floats" ), FUDGE_OK );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_FLOAT_ARRAY );

    TEST_EQUALS_INT( FudgeMsg_getFieldByName ( &field, submessage, "Empty SubMessage" ), FUDGE_INVALID_NAME );
    TEST_EQUALS_INT( FudgeMsg_getFieldByOrdinal ( &field, submessage, 1 ), FUDGE_INVALID_ORDINAL );

    /* Check the empty message */
    TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( &field, message, 13 ), FUDGE_OK );
    submessage = field.data.message;
    TEST_EQUALS_INT( FudgeMsg_numFields ( submessage ), 0 );

    /* Clean up */
    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );
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

DEFINE_TEST_SUITE( Message )
    REGISTER_TEST( FieldFunctions )
    REGISTER_TEST( IntegerFieldDowncasting )
END_TEST_SUITE

