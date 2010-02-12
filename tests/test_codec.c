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
#include "fudge/codec.h"
#include "simpletest.h"

#define ALLNAMES_FILENAME       "test_data/allNames.dat"
#define FIXED_WIDTH_FILENAME    "test_data/fixedWidthByteArrays.dat"
#define ALLORDINALS_FILENAME    "test_data/allOrdinals.dat"
#define SUBMSG_FILENAME         "test_data/subMsg.dat"
#define UNKNOWN_FILENAME        "test_data/unknown.dat"
#define VARIABLE_WIDTH_FILENAME "test_data/variableWidthColumnSizes.dat"
#define DEEPER_FILENAME         "test_data/deeper_fudge_msg.dat"

FudgeMsg loadFudgeMsg ( const char * filename );
void loadFile ( fudge_byte * * target, fudge_i32 * targetSize, const char * filename );
FudgeStatus appendBytes ( fudge_byte * * target, fudge_i32 * targetSize, fudge_byte * source, fudge_i32 sourceSize );

DEFINE_TEST( DecodeAllNames )
    FudgeField fields [ 32 ];
    fudge_byte empty [ 8192 ];
    int index;
    FudgeMsg message = loadFudgeMsg ( ALLNAMES_FILENAME );

    /* All the arrays in this test are empty */
    memset ( empty, 0, sizeof ( empty ) );

    /* Check message contents */
    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 21 );
    for ( index = 0; index < FudgeMsg_numFields ( message ); ++index )
    {
        TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( fields + index, message, index ), FUDGE_OK );
        TEST_EQUALS_TRUE( fields [ index ].flags & FUDGE_FIELD_HAS_NAME );
        TEST_EQUALS_TRUE( ! ( fields [ index ].flags & FUDGE_FIELD_HAS_ORDINAL ) );
    }

    TEST_EQUALS_INT( fields [ 0 ].type, FUDGE_TYPE_BOOLEAN );   TEST_EQUALS_MEMORY( fields [ 0 ].name, 7, "boolean", 7 );   TEST_EQUALS_TRUE( fields [ 0 ].data.boolean ); 
    TEST_EQUALS_INT( fields [ 1 ].type, FUDGE_TYPE_BOOLEAN );   TEST_EQUALS_MEMORY( fields [ 1 ].name, 7, "Boolean", 7 );   TEST_EQUALS_TRUE( ! fields [ 1 ].data.boolean ); 
    TEST_EQUALS_INT( fields [ 2 ].type, FUDGE_TYPE_BYTE );      TEST_EQUALS_MEMORY( fields [ 2 ].name, 4, "byte", 4 );      TEST_EQUALS_INT( fields [ 2 ].data.byte, 5 ); 
    TEST_EQUALS_INT( fields [ 3 ].type, FUDGE_TYPE_BYTE );      TEST_EQUALS_MEMORY( fields [ 3 ].name, 4, "Byte", 4 );      TEST_EQUALS_INT( fields [ 3 ].data.byte, 5 ); 
    TEST_EQUALS_INT( fields [ 4 ].type, FUDGE_TYPE_SHORT );     TEST_EQUALS_MEMORY( fields [ 4 ].name, 5, "short", 5 );     TEST_EQUALS_INT( fields [ 4 ].data.i16, 127 + 5 );
    TEST_EQUALS_INT( fields [ 5 ].type, FUDGE_TYPE_SHORT );     TEST_EQUALS_MEMORY( fields [ 5 ].name, 5, "Short", 5 );     TEST_EQUALS_INT( fields [ 5 ].data.i16, 127 + 5 );
    TEST_EQUALS_INT( fields [ 6 ].type, FUDGE_TYPE_INT );       TEST_EQUALS_MEMORY( fields [ 6 ].name, 3, "int", 3 );       TEST_EQUALS_INT( fields [ 6 ].data.i32, 32767 + 5 );
    TEST_EQUALS_INT( fields [ 7 ].type, FUDGE_TYPE_INT );       TEST_EQUALS_MEMORY( fields [ 7 ].name, 7, "Integer", 7 );   TEST_EQUALS_INT( fields [ 7 ].data.i32, 32767 + 5 );
    TEST_EQUALS_INT( fields [ 8 ].type, FUDGE_TYPE_LONG );      TEST_EQUALS_MEMORY( fields [ 8 ].name, 4, "long", 4 );      TEST_EQUALS_INT( fields [ 8 ].data.i64, 2147483647ll + 5ll );
    TEST_EQUALS_INT( fields [ 9 ].type, FUDGE_TYPE_LONG );      TEST_EQUALS_MEMORY( fields [ 9 ].name, 4, "Long", 4 );      TEST_EQUALS_INT( fields [ 9 ].data.i64, 2147483647ll + 5ll );;
    TEST_EQUALS_INT( fields [ 10 ].type, FUDGE_TYPE_FLOAT );    TEST_EQUALS_MEMORY( fields [ 10 ].name, 5, "float", 5 );    TEST_EQUALS_FLOAT( fields [ 10 ].data.f32, 0.5, 0.00001 ); 
    TEST_EQUALS_INT( fields [ 11 ].type, FUDGE_TYPE_FLOAT );    TEST_EQUALS_MEMORY( fields [ 11 ].name, 5, "Float", 5 );    TEST_EQUALS_FLOAT( fields [ 11 ].data.f32, 0.5, 0.00001 ); 
    TEST_EQUALS_INT( fields [ 12 ].type, FUDGE_TYPE_DOUBLE );   TEST_EQUALS_MEMORY( fields [ 12 ].name, 6, "double", 6 );   TEST_EQUALS_FLOAT( fields [ 12 ].data.f64, 0.27362, 0.000001 ); 
    TEST_EQUALS_INT( fields [ 13 ].type, FUDGE_TYPE_DOUBLE );   TEST_EQUALS_MEMORY( fields [ 13 ].name, 6, "Double", 6 );   TEST_EQUALS_FLOAT( fields [ 13 ].data.f64, 0.27362, 0.000001 ); 

    TEST_EQUALS_INT( fields [ 14 ].type, FUDGE_TYPE_STRING );   TEST_EQUALS_MEMORY( fields [ 14 ].name, 6, "String", 6 );   TEST_EQUALS_MEMORY( fields [ 14 ].data.bytes, fields [ 14 ].numbytes, "Kirk Wylie", 10 );

    TEST_EQUALS_INT( fields [ 15 ].type, FUDGE_TYPE_FLOAT_ARRAY );   TEST_EQUALS_MEMORY( fields [ 15 ].name, 11, "float array", 11 );   TEST_EQUALS_MEMORY( fields [ 15 ].data.bytes, fields [ 15 ].numbytes, empty, 24 * sizeof ( fudge_f32 ) );
    TEST_EQUALS_INT( fields [ 16 ].type, FUDGE_TYPE_DOUBLE_ARRAY );  TEST_EQUALS_MEMORY( fields [ 16 ].name, 12, "double array", 12 );  TEST_EQUALS_MEMORY( fields [ 16 ].data.bytes, fields [ 16 ].numbytes, empty, 273 * sizeof ( fudge_f64 ) );
    TEST_EQUALS_INT( fields [ 17 ].type, FUDGE_TYPE_SHORT_ARRAY );   TEST_EQUALS_MEMORY( fields [ 17 ].name, 11, "short array", 11 );   TEST_EQUALS_MEMORY( fields [ 17 ].data.bytes, fields [ 17 ].numbytes, empty, 32 * sizeof ( fudge_i16 ) );
    TEST_EQUALS_INT( fields [ 18 ].type, FUDGE_TYPE_INT_ARRAY );     TEST_EQUALS_MEMORY( fields [ 18 ].name, 9, "int array", 9 );       TEST_EQUALS_MEMORY( fields [ 18 ].data.bytes, fields [ 18 ].numbytes, empty, 83 * sizeof ( fudge_i32 ) );
    TEST_EQUALS_INT( fields [ 19 ].type, FUDGE_TYPE_LONG_ARRAY );    TEST_EQUALS_MEMORY( fields [ 19 ].name, 10, "long array", 10 );    TEST_EQUALS_MEMORY( fields [ 19 ].data.bytes, fields [ 19 ].numbytes, empty, 837 * sizeof ( fudge_i64 ) );

    TEST_EQUALS_INT( fields [ 20 ].type, FUDGE_TYPE_INDICATOR );    TEST_EQUALS_MEMORY( fields [ 20 ].name, 9, "indicator", 9 );

    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );
END_TEST

DEFINE_TEST( DecodeFixedWidths )
    FudgeField fields [ 16 ];
    fudge_byte bytes [ 512 ];
    int index;
    FudgeMsg message = loadFudgeMsg ( FIXED_WIDTH_FILENAME );

    /* The arrays are populated in this test */
    for ( index = 0; index < sizeof ( bytes ); ++index )
        bytes [ index ] = ( fudge_byte ) index;

    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 10 );
    for ( index = 0; index < FudgeMsg_numFields ( message ); ++index )
    {
        TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( fields + index, message, index ), FUDGE_OK );
        TEST_EQUALS_TRUE( fields [ index ].flags & FUDGE_FIELD_HAS_NAME );
        TEST_EQUALS_TRUE( ! ( fields [ index ].flags & FUDGE_FIELD_HAS_ORDINAL ) );
    }

    TEST_EQUALS_INT( fields [ 0 ].type, FUDGE_TYPE_BYTE_ARRAY_4 );      TEST_EQUALS_MEMORY( fields [ 0 ].name, 7, "byte[4]", 7 );    TEST_EQUALS_MEMORY( fields [ 0 ].data.bytes, fields [ 0 ].numbytes, bytes, 4 );
    TEST_EQUALS_INT( fields [ 1 ].type, FUDGE_TYPE_BYTE_ARRAY_8 );      TEST_EQUALS_MEMORY( fields [ 1 ].name, 7, "byte[8]", 7 );    TEST_EQUALS_MEMORY( fields [ 1 ].data.bytes, fields [ 1 ].numbytes, bytes, 8 );
    TEST_EQUALS_INT( fields [ 2 ].type, FUDGE_TYPE_BYTE_ARRAY_16 );     TEST_EQUALS_MEMORY( fields [ 2 ].name, 8, "byte[16]", 8 );   TEST_EQUALS_MEMORY( fields [ 2 ].data.bytes, fields [ 2 ].numbytes, bytes, 16 );
    TEST_EQUALS_INT( fields [ 3 ].type, FUDGE_TYPE_BYTE_ARRAY_20 );     TEST_EQUALS_MEMORY( fields [ 3 ].name, 8, "byte[20]", 8 );   TEST_EQUALS_MEMORY( fields [ 3 ].data.bytes, fields [ 3 ].numbytes, bytes, 20 );
    TEST_EQUALS_INT( fields [ 4 ].type, FUDGE_TYPE_BYTE_ARRAY_32 );     TEST_EQUALS_MEMORY( fields [ 4 ].name, 8, "byte[32]", 8 );   TEST_EQUALS_MEMORY( fields [ 4 ].data.bytes, fields [ 4 ].numbytes, bytes, 32 );
    TEST_EQUALS_INT( fields [ 5 ].type, FUDGE_TYPE_BYTE_ARRAY_64 );     TEST_EQUALS_MEMORY( fields [ 5 ].name, 8, "byte[64]", 8 );   TEST_EQUALS_MEMORY( fields [ 5 ].data.bytes, fields [ 5 ].numbytes, bytes, 64 );
    TEST_EQUALS_INT( fields [ 6 ].type, FUDGE_TYPE_BYTE_ARRAY_128 );    TEST_EQUALS_MEMORY( fields [ 6 ].name, 9, "byte[128]", 9 );  TEST_EQUALS_MEMORY( fields [ 6 ].data.bytes, fields [ 6 ].numbytes, bytes, 128 );
    TEST_EQUALS_INT( fields [ 7 ].type, FUDGE_TYPE_BYTE_ARRAY_256 );    TEST_EQUALS_MEMORY( fields [ 7 ].name, 9, "byte[256]", 9 );  TEST_EQUALS_MEMORY( fields [ 7 ].data.bytes, fields [ 7 ].numbytes, bytes, 256 );
    TEST_EQUALS_INT( fields [ 8 ].type, FUDGE_TYPE_BYTE_ARRAY_512 );    TEST_EQUALS_MEMORY( fields [ 8 ].name, 9, "byte[512]", 9 );  TEST_EQUALS_MEMORY( fields [ 8 ].data.bytes, fields [ 8 ].numbytes, bytes, 512 );
    
    TEST_EQUALS_INT( fields [ 9 ].type, FUDGE_TYPE_BYTE_ARRAY );        TEST_EQUALS_MEMORY( fields [ 9 ].name, 8, "byte[28]", 8 );   TEST_EQUALS_MEMORY( fields [ 9 ].data.bytes, fields [ 9 ].numbytes, bytes, 28 );

    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );
END_TEST

DEFINE_TEST( DecodeAllOrdinals )
    FudgeField fields [ 32 ];
    fudge_byte empty [ 4096 ];
    int index;
    FudgeMsg message = loadFudgeMsg ( ALLORDINALS_FILENAME );
    
    memset ( empty, 0, sizeof ( empty ) );

    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 17 );
    TEST_EQUALS_INT( FudgeMsg_getFields ( fields, sizeof ( fields ) / sizeof ( FudgeField ), message ), 17 );
    for ( index = 0; index < FudgeMsg_numFields ( message ); ++index )
    {
        TEST_EQUALS_TRUE( ! ( fields [ index ].flags & FUDGE_FIELD_HAS_NAME ) );
        TEST_EQUALS_TRUE( fields [ index ].flags & FUDGE_FIELD_HAS_ORDINAL );
    }

    TEST_EQUALS_INT( fields [ 0 ].type, FUDGE_TYPE_BOOLEAN );   TEST_EQUALS_INT( fields [ 0 ].ordinal, 1 );     TEST_EQUALS_TRUE( fields [ 0 ].data.boolean ); 
    TEST_EQUALS_INT( fields [ 1 ].type, FUDGE_TYPE_BOOLEAN );   TEST_EQUALS_INT( fields [ 1 ].ordinal, 2 );     TEST_EQUALS_TRUE( ! fields [ 1 ].data.boolean ); 
    TEST_EQUALS_INT( fields [ 2 ].type, FUDGE_TYPE_BYTE );      TEST_EQUALS_INT( fields [ 2 ].ordinal, 3 );     TEST_EQUALS_INT( fields [ 2 ].data.byte, 5 ); 
    TEST_EQUALS_INT( fields [ 3 ].type, FUDGE_TYPE_BYTE );      TEST_EQUALS_INT( fields [ 3 ].ordinal, 4 );     TEST_EQUALS_INT( fields [ 3 ].data.byte, 5 ); 
    TEST_EQUALS_INT( fields [ 4 ].type, FUDGE_TYPE_SHORT );     TEST_EQUALS_INT( fields [ 4 ].ordinal, 5 );     TEST_EQUALS_INT( fields [ 4 ].data.i16, 127 + 5 );
    TEST_EQUALS_INT( fields [ 5 ].type, FUDGE_TYPE_SHORT );     TEST_EQUALS_INT( fields [ 5 ].ordinal, 6 );     TEST_EQUALS_INT( fields [ 5 ].data.i16, 127 + 5 );
    TEST_EQUALS_INT( fields [ 6 ].type, FUDGE_TYPE_INT );       TEST_EQUALS_INT( fields [ 6 ].ordinal, 7 );     TEST_EQUALS_INT( fields [ 6 ].data.i32, 32767 + 5 );
    TEST_EQUALS_INT( fields [ 7 ].type, FUDGE_TYPE_INT );       TEST_EQUALS_INT( fields [ 7 ].ordinal, 8 );     TEST_EQUALS_INT( fields [ 7 ].data.i32, 32767 + 5 );
    TEST_EQUALS_INT( fields [ 8 ].type, FUDGE_TYPE_LONG );      TEST_EQUALS_INT( fields [ 8 ].ordinal, 9 );     TEST_EQUALS_INT( fields [ 8 ].data.i64, 2147483647ll + 5ll );
    TEST_EQUALS_INT( fields [ 9 ].type, FUDGE_TYPE_LONG );      TEST_EQUALS_INT( fields [ 9 ].ordinal, 10 );    TEST_EQUALS_INT( fields [ 9 ].data.i64, 2147483647ll + 5ll );
    TEST_EQUALS_INT( fields [ 10 ].type, FUDGE_TYPE_FLOAT );    TEST_EQUALS_INT( fields [ 10 ].ordinal, 11 );   TEST_EQUALS_FLOAT( fields [ 10 ].data.f32, 0.5, 0.00001 ); 
    TEST_EQUALS_INT( fields [ 11 ].type, FUDGE_TYPE_FLOAT );    TEST_EQUALS_INT( fields [ 11 ].ordinal, 12 );   TEST_EQUALS_FLOAT( fields [ 11 ].data.f32, 0.5, 0.00001 ); 
    TEST_EQUALS_INT( fields [ 12 ].type, FUDGE_TYPE_DOUBLE );   TEST_EQUALS_INT( fields [ 12 ].ordinal, 13 );   TEST_EQUALS_FLOAT( fields [ 12 ].data.f64, 0.27362, 0.000001 ); 
    TEST_EQUALS_INT( fields [ 13 ].type, FUDGE_TYPE_DOUBLE );   TEST_EQUALS_INT( fields [ 13 ].ordinal, 14 );   TEST_EQUALS_FLOAT( fields [ 13 ].data.f64, 0.27362, 0.000001 ); 

    TEST_EQUALS_INT( fields [ 14 ].type, FUDGE_TYPE_STRING );   TEST_EQUALS_INT( fields [ 14 ].ordinal, 15 );   TEST_EQUALS_MEMORY( fields [ 14 ].data.bytes, fields [ 14 ].numbytes, "Kirk Wylie", 10 );

    TEST_EQUALS_INT( fields [ 15 ].type, FUDGE_TYPE_FLOAT_ARRAY );   TEST_EQUALS_INT( fields [ 15 ].ordinal, 16 );   TEST_EQUALS_MEMORY( fields [ 15 ].data.bytes, fields [ 15 ].numbytes, empty, 24 * sizeof ( fudge_f32 ) );
    TEST_EQUALS_INT( fields [ 16 ].type, FUDGE_TYPE_DOUBLE_ARRAY );  TEST_EQUALS_INT( fields [ 16 ].ordinal, 17 );   TEST_EQUALS_MEMORY( fields [ 16 ].data.bytes, fields [ 16 ].numbytes, empty, 273 * sizeof ( fudge_f64 ) );

    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );
END_TEST

DEFINE_TEST( DecodeSubMsgs )
    FudgeField field;
    FudgeMsg message = loadFudgeMsg ( SUBMSG_FILENAME );
    FudgeMsg submessage;

    /* Check message contents */
    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 2 );

    TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( &field, message, 0 ), FUDGE_OK );   TEST_EQUALS_INT( field.type, FUDGE_TYPE_FUDGE_MSG );    TEST_EQUALS_MEMORY( field.name, 4, "sub1", 4 );
    TEST_EQUALS_TRUE( ( submessage = field.data.message ) != 0 );
    TEST_EQUALS_INT( FudgeMsg_numFields ( submessage ), 2 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( &field, submessage, 0 ), FUDGE_OK );    TEST_EQUALS_INT( field.type, FUDGE_TYPE_STRING );   TEST_EQUALS_MEMORY( field.name, 6, "bibble", 6 );   TEST_EQUALS_MEMORY( field.data.bytes, field.numbytes, "fibble", 6 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( &field, submessage, 1 ), FUDGE_OK );    TEST_EQUALS_INT( field.type, FUDGE_TYPE_STRING );   TEST_EQUALS_INT( field.ordinal, 827 );              TEST_EQUALS_MEMORY( field.data.bytes, field.numbytes, "Blibble", 7 );

    TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( &field, message, 1 ), FUDGE_OK );   TEST_EQUALS_INT( field.type, FUDGE_TYPE_FUDGE_MSG );    TEST_EQUALS_MEMORY( field.name, 4, "sub2", 4 );
    TEST_EQUALS_TRUE( ( submessage = field.data.message ) != 0 );
    TEST_EQUALS_INT( FudgeMsg_numFields ( submessage ), 2 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( &field, submessage, 0 ), FUDGE_OK );    TEST_EQUALS_INT( field.type, FUDGE_TYPE_INT );      TEST_EQUALS_MEMORY( field.name, 7, "bibble9", 7 );  TEST_EQUALS_INT( field.data.i32, 9837438 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( &field, submessage, 1 ), FUDGE_OK );    TEST_EQUALS_INT( field.type, FUDGE_TYPE_FLOAT);     TEST_EQUALS_INT( field.ordinal, 828 );              TEST_EQUALS_FLOAT( field.data.f32, 82.77, 0.00001 );

    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );
END_TEST

DEFINE_TEST( DecodeUnknown )
    fudge_byte empty [ 16 ];
    FudgeField field;
    FudgeMsg message = loadFudgeMsg ( UNKNOWN_FILENAME );

    memset ( empty, 0, sizeof ( empty ) );

    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 1 );
    TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( &field, message, 0 ), FUDGE_OK );
    TEST_EQUALS_INT( field.type, 200 );
    TEST_EQUALS_MEMORY( field.name, 7, "unknown", 7 );
    TEST_EQUALS_MEMORY( field.data.bytes, field.numbytes, empty, 10 );

    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );
END_TEST

DEFINE_TEST( DecodeVariableWidths )
    fudge_byte empty [ 131072 ];
    int index;
    FudgeField fields [ 8 ];
    FudgeMsg message = loadFudgeMsg ( VARIABLE_WIDTH_FILENAME );

    /* All the arrays in this test are empty */
    memset ( empty, 0, sizeof ( empty ) );

    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 3 );
    for ( index = 0; index < FudgeMsg_numFields ( message ); ++index )
    {
        TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( fields + index, message, index ), FUDGE_OK );
        TEST_EQUALS_INT( fields [ index ].type, FUDGE_TYPE_BYTE_ARRAY );
        TEST_EQUALS_TRUE( fields [ index ].flags & FUDGE_FIELD_HAS_NAME );
        TEST_EQUALS_TRUE( ! ( fields [ index ].flags & FUDGE_FIELD_HAS_ORDINAL ) );
    }

    TEST_EQUALS_MEMORY( fields [ 0 ].name, 3, "100", 3 );    TEST_EQUALS_MEMORY( fields [ 0 ].data.bytes, fields [ 0 ].numbytes, empty, 100 );
    TEST_EQUALS_MEMORY( fields [ 1 ].name, 4, "1000", 4 );   TEST_EQUALS_MEMORY( fields [ 1 ].data.bytes, fields [ 1 ].numbytes, empty, 1000 );
    TEST_EQUALS_MEMORY( fields [ 2 ].name, 5, "10000", 5 );  TEST_EQUALS_MEMORY( fields [ 2 ].data.bytes, fields [ 2 ].numbytes, empty, 100000 );

    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );
END_TEST

DEFINE_TEST( DecodeDeepTree )
    int index;
    FudgeField fields [ 32 ];
    fudge_byte bytes [ 512 ], empty [ 128 ];
    fudge_i16 shorts [ 16 ];
    fudge_f64 doubles [ 16 ];
    FudgeMsg bytemessage, arraysmessage, fparraymessage, emptymessage;
    FudgeMsg message = loadFudgeMsg ( DEEPER_FILENAME );

    /* Populate the comparison arrays */
    for ( index = 0; index < sizeof ( bytes ); ++index ) bytes [ index ] = index;
    for ( index = 0; index < sizeof ( empty ); ++index ) empty [ index ] = 0;
    for ( index = 0; index < sizeof ( shorts ) / sizeof ( fudge_i16 ); ++index ) shorts [ index ] = index;
    for ( index = 0; index < sizeof ( doubles ) / sizeof ( fudge_f64 ); ++index ) doubles [ index ] = ( ( fudge_f64 ) index ) / 10.0;

    /* Check the top-level fields first */
    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 13 );
    for ( index = 0; index < FudgeMsg_numFields ( message ); ++index )
    {
        TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( fields + index, message, index ), FUDGE_OK );
        TEST_EQUALS_TRUE( fields [ index ].flags & FUDGE_FIELD_HAS_NAME );
        TEST_EQUALS_TRUE( ! ( fields [ index ].flags & FUDGE_FIELD_HAS_ORDINAL ) );
    }

    TEST_EQUALS_INT( fields [  0 ].type, FUDGE_TYPE_INDICATOR );    TEST_EQUALS_MEMORY( fields [  0 ].name, 9, "Indicator", 9 );
    TEST_EQUALS_INT( fields [  1 ].type, FUDGE_TYPE_BOOLEAN );      TEST_EQUALS_MEMORY( fields [  1 ].name, 7, "Boolean", 7 );          TEST_EQUALS_TRUE( fields [ 1 ].data.boolean );
    TEST_EQUALS_INT( fields [  2 ].type, FUDGE_TYPE_BYTE );         TEST_EQUALS_MEMORY( fields [  2 ].name, 4, "Byte", 4 );             TEST_EQUALS_INT( fields [ 2 ].data.byte, -128 );
    TEST_EQUALS_INT( fields [  3 ].type, FUDGE_TYPE_SHORT );        TEST_EQUALS_MEMORY( fields [  3 ].name, 5, "Short", 5 );            TEST_EQUALS_INT( fields [ 3 ].data.i16, -32768 );
    TEST_EQUALS_INT( fields [  4 ].type, FUDGE_TYPE_INT );          TEST_EQUALS_MEMORY( fields [  4 ].name, 3, "Int", 3 );              TEST_EQUALS_INT( fields [ 4 ].data.i32, 2147483647 );
    TEST_EQUALS_INT( fields [  5 ].type, FUDGE_TYPE_LONG );         TEST_EQUALS_MEMORY( fields [  5 ].name, 4, "Long", 4 );             TEST_EQUALS_INT( fields [ 5 ].data.i64, 9223372036854775807ll );
    TEST_EQUALS_INT( fields [  6 ].type, FUDGE_TYPE_FLOAT );        TEST_EQUALS_MEMORY( fields [  6 ].name, 5, "Float", 5 );            TEST_EQUALS_FLOAT( fields [ 6 ].data.f32, 1.2346, 0.0001 );
    TEST_EQUALS_INT( fields [  8 ].type, FUDGE_TYPE_FUDGE_MSG );    TEST_EQUALS_MEMORY( fields [  8 ].name, 10, "ByteArrays", 10 );     TEST_EQUALS_TRUE( ( bytemessage = fields [ 8 ].data.message ) != 0 );
    TEST_EQUALS_INT( fields [  7 ].type, FUDGE_TYPE_DOUBLE );       TEST_EQUALS_MEMORY( fields [  7 ].name, 6, "Double", 6 );           TEST_EQUALS_FLOAT( fields [ 7 ].data.f64, 1.2345678, 0.000001 );
    TEST_EQUALS_INT( fields [  9 ].type, FUDGE_TYPE_STRING );       TEST_EQUALS_MEMORY( fields [  9 ].name, 12, "Empty String", 12 );   TEST_EQUALS_MEMORY( fields [ 9 ].data.bytes, fields [ 9 ].numbytes, 0, 0 );
    TEST_EQUALS_INT( fields [ 10 ].type, FUDGE_TYPE_STRING );       TEST_EQUALS_MEMORY( fields [ 10 ].name, 6, "String", 6 );           TEST_EQUALS_MEMORY( fields [ 10 ].data.bytes, fields [ 10 ].numbytes, "This is a string.", 17 );
    TEST_EQUALS_INT( fields [ 11 ].type, FUDGE_TYPE_FUDGE_MSG );    TEST_EQUALS_MEMORY( fields [ 11 ].name, 6, "Arrays", 6 );           TEST_EQUALS_TRUE( ( arraysmessage = fields [ 11 ].data.message ) != 0 );
    TEST_EQUALS_INT( fields [ 12 ].type, FUDGE_TYPE_FUDGE_MSG );    TEST_EQUALS_MEMORY( fields [ 12 ].name, 12, "Null Message", 12 );   TEST_EQUALS_TRUE( ( emptymessage = fields [ 12 ].data.message ) != 0 );

    /* Check the fixed byte array message */
    TEST_EQUALS_INT( FudgeMsg_numFields ( bytemessage ), 9 );
    TEST_EQUALS_INT( FudgeMsg_getFields ( fields, FudgeMsg_numFields ( bytemessage ), bytemessage ), 9 );
    for ( index = 0; index < FudgeMsg_numFields ( bytemessage ); ++index )
    {
        TEST_EQUALS_TRUE( ! ( fields [ index ].flags & FUDGE_FIELD_HAS_NAME ) );
        TEST_EQUALS_TRUE( fields [ index ].flags & FUDGE_FIELD_HAS_ORDINAL );
    }

    TEST_EQUALS_INT( fields [ 0 ].type, FUDGE_TYPE_BYTE_ARRAY_4 );      TEST_EQUALS_INT( fields [ 0 ].ordinal, 4 );     TEST_EQUALS_MEMORY( fields [ 0 ].data.bytes, fields [ 0 ].numbytes, bytes, 4 );
    TEST_EQUALS_INT( fields [ 1 ].type, FUDGE_TYPE_BYTE_ARRAY_8 );      TEST_EQUALS_INT( fields [ 1 ].ordinal, 8 );     TEST_EQUALS_MEMORY( fields [ 1 ].data.bytes, fields [ 1 ].numbytes, bytes, 8 );
    TEST_EQUALS_INT( fields [ 2 ].type, FUDGE_TYPE_BYTE_ARRAY_16 );     TEST_EQUALS_INT( fields [ 2 ].ordinal, 16 );    TEST_EQUALS_MEMORY( fields [ 2 ].data.bytes, fields [ 2 ].numbytes, bytes, 16 );
    TEST_EQUALS_INT( fields [ 3 ].type, FUDGE_TYPE_BYTE_ARRAY_20 );     TEST_EQUALS_INT( fields [ 3 ].ordinal, 20 );    TEST_EQUALS_MEMORY( fields [ 3 ].data.bytes, fields [ 3 ].numbytes, bytes, 20 );
    TEST_EQUALS_INT( fields [ 4 ].type, FUDGE_TYPE_BYTE_ARRAY_32 );     TEST_EQUALS_INT( fields [ 4 ].ordinal, 32 );    TEST_EQUALS_MEMORY( fields [ 4 ].data.bytes, fields [ 4 ].numbytes, bytes, 32 );
    TEST_EQUALS_INT( fields [ 5 ].type, FUDGE_TYPE_BYTE_ARRAY_64 );     TEST_EQUALS_INT( fields [ 5 ].ordinal, 64 );    TEST_EQUALS_MEMORY( fields [ 5 ].data.bytes, fields [ 5 ].numbytes, bytes, 64 );
    TEST_EQUALS_INT( fields [ 6 ].type, FUDGE_TYPE_BYTE_ARRAY_128 );    TEST_EQUALS_INT( fields [ 6 ].ordinal, 128 );   TEST_EQUALS_MEMORY( fields [ 6 ].data.bytes, fields [ 6 ].numbytes, bytes, 128 );
    TEST_EQUALS_INT( fields [ 7 ].type, FUDGE_TYPE_BYTE_ARRAY_256 );    TEST_EQUALS_INT( fields [ 7 ].ordinal, 256 );   TEST_EQUALS_MEMORY( fields [ 7 ].data.bytes, fields [ 7 ].numbytes, bytes, 256 );
    TEST_EQUALS_INT( fields [ 8 ].type, FUDGE_TYPE_BYTE_ARRAY_512 );    TEST_EQUALS_INT( fields [ 8 ].ordinal, 512 );   TEST_EQUALS_MEMORY( fields [ 8 ].data.bytes, fields [ 8 ].numbytes, bytes, 512 );

    /* Check the arrays message */
    TEST_EQUALS_INT( FudgeMsg_numFields ( arraysmessage ), 9 );
    for ( index = 0; index < FudgeMsg_numFields ( arraysmessage ); ++index )
    {
        TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( fields + index, arraysmessage, index ), FUDGE_OK );
        TEST_EQUALS_TRUE( fields [ index ].flags & FUDGE_FIELD_HAS_NAME );
        TEST_EQUALS_TRUE( ! ( fields [ index ].flags & FUDGE_FIELD_HAS_ORDINAL ) );
    }
    
    TEST_EQUALS_INT( fields [ 0 ].type, FUDGE_TYPE_BYTE_ARRAY );    TEST_EQUALS_MEMORY( fields [ 0 ].name, 7, "Byte[0]", 7 );       TEST_EQUALS_MEMORY( fields [ 0 ].data.bytes, fields [ 0 ].numbytes, 0, 0 );
    TEST_EQUALS_INT( fields [ 1 ].type, FUDGE_TYPE_BYTE_ARRAY );    TEST_EQUALS_MEMORY( fields [ 1 ].name, 8, "Byte[15]", 8 );      TEST_EQUALS_MEMORY( fields [ 1 ].data.bytes, fields [ 1 ].numbytes, bytes, 15 );
    TEST_EQUALS_INT( fields [ 2 ].type, FUDGE_TYPE_FUDGE_MSG );     TEST_EQUALS_MEMORY( fields [ 2 ].name, 9, "FP Arrays", 9 );     TEST_EQUALS_TRUE( ( fparraymessage = fields [ 2 ].data.message ) != 0 );
    TEST_EQUALS_INT( fields [ 3 ].type, FUDGE_TYPE_SHORT_ARRAY );   TEST_EQUALS_MEMORY( fields [ 3 ].name, 8, "Short[0]", 8 );      TEST_EQUALS_MEMORY( fields [ 3 ].data.bytes, fields [ 3 ].numbytes, 0, 0 );
    TEST_EQUALS_INT( fields [ 4 ].type, FUDGE_TYPE_SHORT_ARRAY );   TEST_EQUALS_MEMORY( fields [ 4 ].name, 9, "Short[15]", 9 );     TEST_EQUALS_MEMORY( fields [ 4 ].data.bytes, fields [ 4 ].numbytes, shorts, 15 * sizeof ( fudge_i16 ) );
    TEST_EQUALS_INT( fields [ 5 ].type, FUDGE_TYPE_INT_ARRAY );     TEST_EQUALS_MEMORY( fields [ 5 ].name, 6, "Int[0]", 6 );        TEST_EQUALS_MEMORY( fields [ 5 ].data.bytes, fields [ 5 ].numbytes, 0, 0 );
    TEST_EQUALS_INT( fields [ 6 ].type, FUDGE_TYPE_INT_ARRAY );     TEST_EQUALS_MEMORY( fields [ 6 ].name, 7, "Int[15]", 7 );       TEST_EQUALS_MEMORY( fields [ 6 ].data.bytes, fields [ 6 ].numbytes, empty, 15 * sizeof ( fudge_i32 ) );
    TEST_EQUALS_INT( fields [ 7 ].type, FUDGE_TYPE_LONG_ARRAY );    TEST_EQUALS_MEMORY( fields [ 7 ].name, 7, "Long[0]", 7 );       TEST_EQUALS_MEMORY( fields [ 7 ].data.bytes, fields [ 7 ].numbytes, 0, 0 );
    TEST_EQUALS_INT( fields [ 8 ].type, FUDGE_TYPE_LONG_ARRAY );    TEST_EQUALS_MEMORY( fields [ 8 ].name, 8, "Long[15]", 8 );      TEST_EQUALS_MEMORY( fields [ 8 ].data.bytes, fields [ 8 ].numbytes, empty, 15 * sizeof ( fudge_i64 ) );

    /* Check the FP arrays message */
    TEST_EQUALS_INT( FudgeMsg_numFields ( fparraymessage ), 4 );
    for ( index = 0; index < FudgeMsg_numFields ( fparraymessage ); ++index )
    {
        TEST_EQUALS_INT( FudgeMsg_getFieldAtIndex ( fields + index, fparraymessage, index ), FUDGE_OK );
        TEST_EQUALS_TRUE( fields [ index ].flags & FUDGE_FIELD_HAS_NAME );
        TEST_EQUALS_TRUE( ! ( fields [ index ].flags & FUDGE_FIELD_HAS_ORDINAL ) );
    }
    
    TEST_EQUALS_INT( fields [ 0 ].type, FUDGE_TYPE_FLOAT_ARRAY );   TEST_EQUALS_MEMORY( fields [ 0 ].name, 8, "Float[0]", 8 );      TEST_EQUALS_MEMORY( fields [ 0 ].data.bytes, fields [ 0 ].numbytes, 0, 0 );
    TEST_EQUALS_INT( fields [ 1 ].type, FUDGE_TYPE_FLOAT_ARRAY );   TEST_EQUALS_MEMORY( fields [ 1 ].name, 9, "Float[15]", 9 );     TEST_EQUALS_MEMORY( fields [ 1 ].data.bytes, fields [ 1 ].numbytes, empty, 15 * sizeof ( fudge_f32 ) );
    TEST_EQUALS_INT( fields [ 2 ].type, FUDGE_TYPE_DOUBLE_ARRAY );  TEST_EQUALS_MEMORY( fields [ 2 ].name, 9, "Double[0]", 9 );     TEST_EQUALS_MEMORY( fields [ 2 ].data.bytes, fields [ 2 ].numbytes, 0, 0 );
    TEST_EQUALS_INT( fields [ 3 ].type, FUDGE_TYPE_DOUBLE_ARRAY );  TEST_EQUALS_MEMORY( fields [ 3 ].name, 10, "Double[15]", 10 );  TEST_EQUALS_MEMORY( fields [ 3 ].data.bytes, fields [ 3 ].numbytes, doubles, 15 * sizeof ( fudge_f64 ) );
    
    /* Check the empty message */
    TEST_EQUALS_INT( FudgeMsg_numFields ( emptymessage ), 0 );

    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );
END_TEST

DEFINE_TEST( EncodeAllNames )
    fudge_byte empty [ 8192 ];
    fudge_byte * encoded, * reference;
    fudge_i32 encodedsize, referencesize;
    FudgeMsgEnvelope envelope;
    FudgeMsg message;

    memset ( empty, 0, sizeof ( empty ) );

    TEST_EQUALS_INT( FudgeMsg_create ( &message ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldBool ( message, ( const fudge_byte * ) "boolean", 7, 0, FUDGE_TRUE ),       FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldBool ( message, ( const fudge_byte * ) "Boolean", 7, 0, FUDGE_FALSE ),      FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldByte ( message, ( const fudge_byte * ) "byte",    4, 0, 5 ),                FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldByte ( message, ( const fudge_byte * ) "Byte",    4, 0, 5 ),                FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI16  ( message, ( const fudge_byte * ) "short",   5, 0, 127 + 5 ),          FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI16  ( message, ( const fudge_byte * ) "Short",   5, 0, 127 + 5 ),          FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI32  ( message, ( const fudge_byte * ) "int",     3, 0, 32767 + 5 ),        FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI32  ( message, ( const fudge_byte * ) "Integer", 7, 0, 32767 + 5 ),        FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI64  ( message, ( const fudge_byte * ) "long",    4, 0, 2147483647ll + 5 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI64  ( message, ( const fudge_byte * ) "Long",    4, 0, 2147483647ll + 5 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF32  ( message, ( const fudge_byte * ) "float",   5, 0, 0.5 ),              FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF32  ( message, ( const fudge_byte * ) "Float",   5, 0, 0.5 ),              FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF64  ( message, ( const fudge_byte * ) "double",  6, 0, 0.27362 ),          FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF64  ( message, ( const fudge_byte * ) "Double",  6, 0, 0.27362 ),          FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_addFieldString ( message, ( const fudge_byte * ) "String", 6, 0, ( const fudge_byte * ) "Kirk Wylie", 10 ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_addFieldF32Array ( message, ( const fudge_byte * ) "float array",  11, 0, ( fudge_f32 * ) empty, 24 ),  FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF64Array ( message, ( const fudge_byte * ) "double array", 12, 0, ( fudge_f64 * ) empty, 273 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI16Array ( message, ( const fudge_byte * ) "short array",  11, 0, ( fudge_i16 * ) empty, 32 ),  FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI32Array ( message, ( const fudge_byte * ) "int array",     9, 0, ( fudge_i32 * ) empty, 83 ),  FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI64Array ( message, ( const fudge_byte * ) "long array",   10, 0, ( fudge_i64 * ) empty, 837 ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_addFieldIndicator ( message, ( const fudge_byte * ) "indicator", 9, 0 ), FUDGE_OK );

    envelope.directives = 0;
    envelope.schemaversion = 0;
    envelope.taxonomy = 0;
    envelope.message = message;

    TEST_EQUALS_INT( FudgeCodec_encodeMsg ( envelope, &encoded, &encodedsize ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );

    loadFile ( &reference, &referencesize, ALLNAMES_FILENAME );
    TEST_EQUALS_MEMORY( encoded, encodedsize, reference, referencesize );

    free ( encoded );
    free ( reference );
END_TEST

DEFINE_TEST( EncodeFixedWidths )
    fudge_byte bytes [ 512 ];
    int index;
    fudge_byte * encoded, * reference;
    fudge_i32 encodedsize, referencesize;
    FudgeMsgEnvelope envelope;
    FudgeMsg message;

    for ( index = 0; index < sizeof ( bytes ); ++index )
        bytes [ index ] = ( fudge_byte ) index;

    TEST_EQUALS_INT( FudgeMsg_create ( &message ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addField4ByteArray   ( message, ( const fudge_byte * ) "byte[4]",   7, 0, bytes ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addField8ByteArray   ( message, ( const fudge_byte * ) "byte[8]",   7, 0, bytes ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addField16ByteArray  ( message, ( const fudge_byte * ) "byte[16]",  8, 0, bytes ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addField20ByteArray  ( message, ( const fudge_byte * ) "byte[20]",  8, 0, bytes ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addField32ByteArray  ( message, ( const fudge_byte * ) "byte[32]",  8, 0, bytes ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addField64ByteArray  ( message, ( const fudge_byte * ) "byte[64]",  8, 0, bytes ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addField128ByteArray ( message, ( const fudge_byte * ) "byte[128]", 9, 0, bytes ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addField256ByteArray ( message, ( const fudge_byte * ) "byte[256]", 9, 0, bytes ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addField512ByteArray ( message, ( const fudge_byte * ) "byte[512]", 9, 0, bytes ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_addFieldByteArray ( message, ( const fudge_byte * ) "byte[28]", 8, 0, bytes, 28 ), FUDGE_OK );

    envelope.directives = 0;
    envelope.schemaversion = 0;
    envelope.taxonomy = 0;
    envelope.message = message;

    TEST_EQUALS_INT( FudgeCodec_encodeMsg ( envelope, &encoded, &encodedsize ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );

    loadFile ( &reference, &referencesize, FIXED_WIDTH_FILENAME );
    TEST_EQUALS_MEMORY( encoded, encodedsize, reference, referencesize );

    free ( encoded );
    free ( reference );
END_TEST

DEFINE_TEST( EncodeAllOrdinals )
    fudge_byte empty [ 4096 ];
    fudge_byte * encoded, * reference;
    fudge_i32 encodedsize, referencesize;
    fudge_i16 ordinal;
    FudgeMsgEnvelope envelope;
    FudgeMsg message;

    memset ( empty, 0, sizeof ( empty ) );

    TEST_EQUALS_INT( FudgeMsg_create ( &message ), FUDGE_OK );
    ordinal = 1;    TEST_EQUALS_INT( FudgeMsg_addFieldBool ( message, 0, 0, &ordinal, FUDGE_TRUE ),       FUDGE_OK );
    ordinal = 2;    TEST_EQUALS_INT( FudgeMsg_addFieldBool ( message, 0, 0, &ordinal, FUDGE_FALSE ),      FUDGE_OK );
    ordinal = 3;    TEST_EQUALS_INT( FudgeMsg_addFieldByte ( message, 0, 0, &ordinal, 5 ),                FUDGE_OK );
    ordinal = 4;    TEST_EQUALS_INT( FudgeMsg_addFieldByte ( message, 0, 0, &ordinal, 5 ),                FUDGE_OK );
    ordinal = 5;    TEST_EQUALS_INT( FudgeMsg_addFieldI16  ( message, 0, 0, &ordinal, 127 + 5 ),          FUDGE_OK );
    ordinal = 6;    TEST_EQUALS_INT( FudgeMsg_addFieldI16  ( message, 0, 0, &ordinal, 127 + 5 ),          FUDGE_OK );
    ordinal = 7;    TEST_EQUALS_INT( FudgeMsg_addFieldI32  ( message, 0, 0, &ordinal, 32767 + 5 ),        FUDGE_OK );
    ordinal = 8;    TEST_EQUALS_INT( FudgeMsg_addFieldI32  ( message, 0, 0, &ordinal, 32767 + 5 ),        FUDGE_OK );
    ordinal = 9;    TEST_EQUALS_INT( FudgeMsg_addFieldI64  ( message, 0, 0, &ordinal, 2147483647ll + 5 ), FUDGE_OK );
    ordinal = 10;   TEST_EQUALS_INT( FudgeMsg_addFieldI64  ( message, 0, 0, &ordinal, 2147483647ll + 5 ), FUDGE_OK );
    ordinal = 11;   TEST_EQUALS_INT( FudgeMsg_addFieldF32  ( message, 0, 0, &ordinal, 0.5 ),              FUDGE_OK );
    ordinal = 12;   TEST_EQUALS_INT( FudgeMsg_addFieldF32  ( message, 0, 0, &ordinal, 0.5 ),              FUDGE_OK );
    ordinal = 13;   TEST_EQUALS_INT( FudgeMsg_addFieldF64  ( message, 0, 0, &ordinal, 0.27362 ),          FUDGE_OK );
    ordinal = 14;   TEST_EQUALS_INT( FudgeMsg_addFieldF64  ( message, 0, 0, &ordinal, 0.27362 ),          FUDGE_OK );

    ordinal = 15;   TEST_EQUALS_INT( FudgeMsg_addFieldString ( message, 0, 0, &ordinal, ( const fudge_byte * ) "Kirk Wylie", 10 ), FUDGE_OK );
    
    ordinal = 16;   TEST_EQUALS_INT( FudgeMsg_addFieldF32Array ( message, 0, 0, &ordinal, ( fudge_f32 * ) empty, 24 ),  FUDGE_OK );
    ordinal = 17;   TEST_EQUALS_INT( FudgeMsg_addFieldF64Array ( message, 0, 0, &ordinal, ( fudge_f64 * ) empty, 273 ), FUDGE_OK );

    envelope.directives = 0;
    envelope.schemaversion = 0;
    envelope.taxonomy = 0;
    envelope.message = message;

    TEST_EQUALS_INT( FudgeCodec_encodeMsg ( envelope, &encoded, &encodedsize ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );

    loadFile ( &reference, &referencesize, ALLORDINALS_FILENAME );
    TEST_EQUALS_MEMORY( encoded, encodedsize, reference, referencesize );

    free ( encoded );
    free ( reference );
END_TEST

DEFINE_TEST( EncodeUnknown )
    fudge_byte empty [ 16 ];
    fudge_byte * encoded, * reference;
    fudge_i32 encodedsize, referencesize;
    FudgeMsgEnvelope envelope;
    FudgeMsg message;

    memset ( empty, 0, sizeof ( empty ) );

    TEST_EQUALS_INT( FudgeMsg_create ( &message ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldOpaque ( message, 200, ( const fudge_byte * ) "unknown", 7, 0, empty, 10 ), FUDGE_OK );

    envelope.directives = 0;
    envelope.schemaversion = 0;
    envelope.taxonomy = 0;
    envelope.message = message;

    TEST_EQUALS_INT( FudgeCodec_encodeMsg ( envelope, &encoded, &encodedsize ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );

    loadFile ( &reference, &referencesize, UNKNOWN_FILENAME );
    TEST_EQUALS_MEMORY( encoded, encodedsize, reference, referencesize );

    free ( encoded );
    free ( reference );
END_TEST

DEFINE_TEST( EncodeSubMsgs )
    fudge_byte * encoded, * reference;
    fudge_i32 encodedsize, referencesize;
    fudge_i16 ordinal;
    FudgeMsgEnvelope envelope;
    FudgeMsg message, submessage;

    TEST_EQUALS_INT( FudgeMsg_create ( &message ), FUDGE_OK );

    /* Create and add the first submessage */
    TEST_EQUALS_INT( FudgeMsg_create ( &submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldString ( submessage, ( const fudge_byte * ) "bibble", 6, 0, ( const fudge_byte * ) "fibble", 6 ), FUDGE_OK );
    ordinal = 827;
    TEST_EQUALS_INT( FudgeMsg_addFieldString ( submessage, 0, 0, &ordinal, ( const fudge_byte * ) "Blibble", 7 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldMsg ( message, ( const fudge_byte * ) "sub1", 4, 0, submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( submessage ), FUDGE_OK );

    /* Create and add the second submessage */
    TEST_EQUALS_INT( FudgeMsg_create ( &submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI32 ( submessage, ( const fudge_byte * ) "bibble9", 7, 0, 9837438 ), FUDGE_OK );
    ordinal = 828;
    TEST_EQUALS_INT( FudgeMsg_addFieldF32 ( submessage, 0, 0, &ordinal, 82.77f ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldMsg ( message, ( const fudge_byte * ) "sub2", 4, 0, submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( submessage ), FUDGE_OK );

    envelope.directives = 0;
    envelope.schemaversion = 0;
    envelope.taxonomy = 0;
    envelope.message = message;

    TEST_EQUALS_INT( FudgeCodec_encodeMsg ( envelope, &encoded, &encodedsize ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );

    loadFile ( &reference, &referencesize, SUBMSG_FILENAME );
    TEST_EQUALS_MEMORY( encoded, encodedsize, reference, referencesize );

    free ( encoded );
    free ( reference );
END_TEST

DEFINE_TEST( EncodeVariableWidths )
    fudge_byte empty [ 100000 ];
    fudge_byte * encoded, * reference;
    fudge_i32 encodedsize, referencesize;
    FudgeMsgEnvelope envelope;
    FudgeMsg message;

    memset ( empty, 0, sizeof ( empty ) );

    TEST_EQUALS_INT( FudgeMsg_create ( &message ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldByteArray ( message, ( const fudge_byte * ) "100", 3, 0, empty, 100 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldByteArray ( message, ( const fudge_byte * ) "1000", 4, 0, empty, 1000 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldByteArray ( message, ( const fudge_byte * ) "10000", 5, 0, empty, 100000 ), FUDGE_OK );

    envelope.directives = 0;
    envelope.schemaversion = 0;
    envelope.taxonomy = 0;
    envelope.message = message;

    TEST_EQUALS_INT( FudgeCodec_encodeMsg ( envelope, &encoded, &encodedsize ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );

    loadFile ( &reference, &referencesize, VARIABLE_WIDTH_FILENAME );
    TEST_EQUALS_MEMORY( encoded, encodedsize, reference, referencesize );

    free ( encoded );
    free ( reference );
END_TEST

DEFINE_TEST( EncodeDeepTree )
    fudge_byte bytes [ 512 ], empty [ 512 ];
    fudge_f64 doubles [ 16 ];
    fudge_i16 shorts [ 16 ];
    fudge_byte * encoded, * reference;
    fudge_i32 encodedsize, referencesize, index;
    fudge_i16 ordinal;
    FudgeMsgEnvelope envelope;
    FudgeMsg message, submessage, subsubmessage;

    memset ( empty, 0, sizeof ( empty ) );
    for ( index = 0; index < sizeof ( bytes ); ++index ) bytes [ index ] = index;
    for ( index = 0; index < sizeof ( doubles ) / sizeof ( fudge_f64 ); ++index ) doubles [ index ] = ( ( fudge_f64 ) index ) / 10.0;
    for ( index = 0; index < sizeof ( shorts ) / sizeof ( fudge_i16 ); ++index ) shorts [ index ] = index;

    TEST_EQUALS_INT( FudgeMsg_create ( &message ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldIndicator ( message, ( const fudge_byte * ) "Indicator", 9, 0 ),                        FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldBool      ( message, ( const fudge_byte * ) "Boolean",   7, 0, FUDGE_TRUE ),            FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldByte      ( message, ( const fudge_byte * ) "Byte",      4, 0, -128 ),                  FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI16       ( message, ( const fudge_byte * ) "Short",     5, 0, -32768 ),                FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI32       ( message, ( const fudge_byte * ) "Int",       3, 0, 2147483647 ),            FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI64       ( message, ( const fudge_byte * ) "Long",      4, 0, 9223372036854775807ll ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF32       ( message, ( const fudge_byte * ) "Float",     5, 0, 1.23456 ),               FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF64       ( message, ( const fudge_byte * ) "Double",    6, 0, 1.2345678 ),             FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_create ( &submessage ), FUDGE_OK );
    ordinal =   4; TEST_EQUALS_INT( FudgeMsg_addField4ByteArray   ( submessage, 0, 0, &ordinal, bytes ), FUDGE_OK );
    ordinal =   8; TEST_EQUALS_INT( FudgeMsg_addField8ByteArray   ( submessage, 0, 0, &ordinal, bytes ), FUDGE_OK );
    ordinal =  16; TEST_EQUALS_INT( FudgeMsg_addField16ByteArray  ( submessage, 0, 0, &ordinal, bytes ), FUDGE_OK );
    ordinal =  20; TEST_EQUALS_INT( FudgeMsg_addField20ByteArray  ( submessage, 0, 0, &ordinal, bytes ), FUDGE_OK );
    ordinal =  32; TEST_EQUALS_INT( FudgeMsg_addField32ByteArray  ( submessage, 0, 0, &ordinal, bytes ), FUDGE_OK );
    ordinal =  64; TEST_EQUALS_INT( FudgeMsg_addField64ByteArray  ( submessage, 0, 0, &ordinal, bytes ), FUDGE_OK );
    ordinal = 128; TEST_EQUALS_INT( FudgeMsg_addField128ByteArray ( submessage, 0, 0, &ordinal, bytes ), FUDGE_OK );
    ordinal = 256; TEST_EQUALS_INT( FudgeMsg_addField256ByteArray ( submessage, 0, 0, &ordinal, bytes ), FUDGE_OK );
    ordinal = 512; TEST_EQUALS_INT( FudgeMsg_addField512ByteArray ( submessage, 0, 0, &ordinal, bytes ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldMsg ( message, ( const fudge_byte * ) "ByteArrays", 10, 0, submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( submessage ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_addFieldString ( message, ( const fudge_byte * ) "Empty String", 12, 0, ( const fudge_byte * ) "",                   0 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldString ( message, ( const fudge_byte * ) "String",        6, 0, ( const fudge_byte * ) "This is a string.", 17 ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_create ( &submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldByteArray ( submessage, ( const fudge_byte * ) "Byte[0]",  7, 0, bytes,  0 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldByteArray ( submessage, ( const fudge_byte * ) "Byte[15]", 8, 0, bytes, 15 ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_create ( &subsubmessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF32Array ( subsubmessage, ( const fudge_byte * ) "Float[0]",    8, 0, ( fudge_f32 * ) empty,  0 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF32Array ( subsubmessage, ( const fudge_byte * ) "Float[15]",   9, 0, ( fudge_f32 * ) empty, 15 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF64Array ( subsubmessage, ( const fudge_byte * ) "Double[0]",   9, 0, doubles,                0 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldF64Array ( subsubmessage, ( const fudge_byte * ) "Double[15]", 10, 0, doubles,               15 ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_addFieldMsg ( submessage, ( const fudge_byte * ) "FP Arrays", 9, 0, subsubmessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( subsubmessage ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_addFieldI16Array ( submessage, ( const fudge_byte * ) "Short[0]",   8, 0, ( fudge_i16 * ) empty,  0 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI16Array ( submessage, ( const fudge_byte * ) "Short[15]",  9, 0, shorts,                15 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI32Array ( submessage, ( const fudge_byte * ) "Int[0]",     6, 0, ( fudge_i32 * ) empty,  0 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI32Array ( submessage, ( const fudge_byte * ) "Int[15]",    7, 0, ( fudge_i32 * ) empty, 15 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI64Array ( submessage, ( const fudge_byte * ) "Long[0]",    7, 0, ( fudge_i64 * ) empty,  0 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldI64Array ( submessage, ( const fudge_byte * ) "Long[15]",   8, 0, ( fudge_i64 * ) empty, 15 ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_addFieldMsg ( message, ( const fudge_byte * ) "Arrays", 6, 0, submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( submessage ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_create ( &submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldMsg ( message, ( const fudge_byte * ) "Null Message", 12, 0, submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( submessage ), FUDGE_OK );

    envelope.directives = 0;
    envelope.schemaversion = 0;
    envelope.taxonomy = 0;
    envelope.message = message;

    TEST_EQUALS_INT( FudgeCodec_encodeMsg ( envelope, &encoded, &encodedsize ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );

    loadFile ( &reference, &referencesize, DEEPER_FILENAME );
    TEST_EQUALS_MEMORY( encoded, encodedsize, reference, referencesize );

    free ( encoded );
    free ( reference );
END_TEST

DEFINE_TEST_SUITE( Codec )
    /* Interop decode test files */
    REGISTER_TEST( DecodeAllNames )
    REGISTER_TEST( DecodeFixedWidths )
    REGISTER_TEST( DecodeAllOrdinals )
    REGISTER_TEST( DecodeSubMsgs )
    REGISTER_TEST( DecodeUnknown )
    REGISTER_TEST( DecodeVariableWidths )

    /* Other decode test files */
    REGISTER_TEST( DecodeDeepTree )

    /* Interop encode tests */
    REGISTER_TEST( EncodeAllNames )
    REGISTER_TEST( EncodeFixedWidths )
    REGISTER_TEST( EncodeAllOrdinals )
    REGISTER_TEST( EncodeUnknown )
    REGISTER_TEST( EncodeSubMsgs )
    REGISTER_TEST( EncodeVariableWidths )

    /* Other encode tests */
    REGISTER_TEST( EncodeDeepTree );
END_TEST_SUITE

FudgeMsg loadFudgeMsg ( const char * filename )
{
    fudge_byte * referenceBytes;
    fudge_i32 numReferenceBytes;
    FudgeMsgEnvelope envelope;

    loadFile ( &referenceBytes, &numReferenceBytes, filename );
    TEST_EQUALS_INT( FudgeCodec_decodeMsg ( &envelope, referenceBytes, numReferenceBytes ), FUDGE_OK );
    free ( referenceBytes );
    return envelope.message;
}

void loadFile ( fudge_byte * * target, fudge_i32 * targetSize, const char * filename )
{
    FILE * file;
    char fullfilename [ FILENAME_MAX ];
    fudge_byte buffer [ 1024 ];
    char * srcdir;

    /* When running "make distcheck" the tests will be triggered from the
       binary directory, which makes the filenames invalid. Helpfully automake
       sets the "srcdir" env var which contains the location of the source
       code directory. */
    if ( ( srcdir = getenv ( "srcdir" ) ) )
        snprintf ( fullfilename, sizeof ( fullfilename ), "%s/%s", srcdir, filename );
    else
        snprintf ( fullfilename, sizeof ( fullfilename ), "%s", filename );

    TEST_EQUALS_TRUE( ( file = fopen ( fullfilename, "rb" ) ) != 0 );
    *targetSize = 0;
    while ( ! feof ( file ) )
    {
        fudge_i32 bytesread = fread ( buffer, 1, sizeof ( buffer ), file );
        if ( bytesread )
            TEST_EQUALS_INT( appendBytes ( target, targetSize, buffer, bytesread ), FUDGE_OK );
    }
    TEST_EQUALS_TRUE( fclose ( file ) == 0 );
}

FudgeStatus appendBytes ( fudge_byte * * target, fudge_i32 * targetSize, fudge_byte * source, fudge_i32 sourceSize )
{
    if ( *targetSize )
    {
        /* Existing data in target array: create an enlarged array and copy across the old data */
        fudge_byte * oldTarget = *target;
        if ( ! ( *target = ( fudge_byte * ) malloc ( sourceSize + *targetSize ) ) )
            return FUDGE_OUT_OF_MEMORY;
        memcpy ( *target, oldTarget, *targetSize );
        free ( oldTarget );
    }
    else
    {
        /* No exisiting data: create an array large enough for the source data */
        if ( ! ( *target = ( fudge_byte * ) malloc ( sourceSize ) ) )
            return FUDGE_OUT_OF_MEMORY;
    }

    /* Append the new data */
    memcpy ( *target + *targetSize, source, sourceSize );
    *targetSize += sourceSize;
    return FUDGE_OK;
}

