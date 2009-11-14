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
#include <math.h>

DEFINE_TEST( FieldFunctions )
    static const fudge_byte rawBytes [ 16 ] = { 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00 };
    static const fudge_i16 rawShorts [ 10 ] = { -32767, 32767, 0, 1, -1, 100, -100, 0, 16385 };
    static const fudge_i32 rawInts [ 4 ] = { 2147483647, 0, -2147483647, 0 };
    static const fudge_i64 rawLongs [ 12 ] = { ( fudge_i64 ) -9223372036854775807ll, 0, ( fudge_i64 ) 9223372036854775807ll, -1, 2, -3, 5, -8, 13, -21, 34, -55 };
    static const fudge_f32 rawFloats [ 8 ] = { 0.0f, 2147483647.0f, 214748364.7f, 21474836.47f, 2147483.647, 2.147483647f, 21.47483647f, 214.7483647f };
    static const fudge_f64 rawDoubles [ 5 ] = { 9223372036854775807.0, 0.0, 0.0000000123456, 1234560000000.0, -9223372036854775807.0 };

    fudge_byte largeByteArray [ 512 ];   /* Will need a large byte array for the fix array testing */
    
    FudgeMsg message, submessage;
    int index;

    /* Populate the large byte array before use it in the tests */
    for ( index = 0; index < sizeof ( largeByteArray ); ++index )
        printf ( "%d\n", largeByteArray [ index ] = index % 256 - 128 );

    /* Construct an empty message */
    TEST_EQUALS_INT( FudgeMsg_create ( &message ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 0 );

    /* Test some failure cases */
    TEST_EQUALS_INT( FudgeMsg_addFieldBool ( 0, 0, FUDGE_FALSE ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeMsg_addFieldMsg ( message, "Null message", 0 ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeMsg_addFieldString ( message, "Null, non zero-length string", 0, 1 ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 0 );

    /* Add indicator, boolean, integer and float fields */
    TEST_EQUALS_INT( FudgeMsg_addFieldIndicator ( message, 0 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldBool ( message, 0, FUDGE_TRUE ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldByte ( message, 0, 127 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI16 ( message, 0, ( fudge_i16 ) 32767 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI32 ( message, 0, ( fudge_i32 ) -2147483647 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI64 ( message, 0, ( fudge_i64 ) 9223372036854775807ll ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF32 ( message, 0, 2147483647.0f ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF64 ( message, 0, -9223372036854775807.0 ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 8 );

    /* TODO Add fixed width byte arrays in a sub message */
    TEST_EQUALS_INT( FudgeMsg_create ( &submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addField4ByteArray ( submessage, 0, largeByteArray ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addField8ByteArray ( submessage, 0, largeByteArray ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addField16ByteArray ( submessage, 0, largeByteArray ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addField20ByteArray ( submessage, 0, largeByteArray ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addField32ByteArray ( submessage, 0, largeByteArray ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addField64ByteArray ( submessage, 0, largeByteArray ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addField128ByteArray ( submessage, 0, largeByteArray ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addField256ByteArray ( submessage, 0, largeByteArray ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addField512ByteArray ( submessage, 0, largeByteArray ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_numFields ( submessage ), 9 );

    TEST_EQUALS_INT( FudgeMsg_addFieldMsg ( message, "Byte Array SubMessage", submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( submessage ), FUDGE_OK );

    /* Add null, empty and populated strings */
    TEST_EQUALS_INT( FudgeMsg_addFieldString ( message, "Null string", 0, 0 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldString ( message, "Empty string", "", 0 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldString ( message, "String", "This is a string", 16 ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 12 );

    /* Add empty and populated arrays in a sub message */
    TEST_EQUALS_INT( FudgeMsg_create ( &submessage ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_addFieldByteArray ( submessage, "No Bytes", 0, 0 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldByteArray ( submessage, "Bytes", rawBytes, 16 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI16Array ( submessage, "Shorts", rawShorts, 10 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI32Array ( submessage, "Ints", rawInts, 4 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI64Array ( submessage, "Longs", rawLongs, 12 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF32Array ( submessage, "Floats", rawFloats, 8 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF64Array ( submessage, "Doubles", rawDoubles, 5 ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_numFields ( submessage ), 7 );

    TEST_EQUALS_INT( FudgeMsg_addFieldMsg ( message, "Array SubMessage", submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( submessage ), FUDGE_OK );

    /* Add an empty sub message */
    TEST_EQUALS_INT( FudgeMsg_create ( &submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldMsg ( message, "Empty SubMessage", submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( submessage ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 14 );

    /* Clean up */
    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );
END_TEST

DEFINE_TEST_SUITE( Message )
    REGISTER_TEST( FieldFunctions )
END_TEST_SUITE

