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
#include "fudge/stringpool.h"
#include "reference.h"
#include "simpletest.h"

#ifndef EXTERNAL_TESTS_ONLY
DEFINE_TEST( ReferenceCount )
    int index;
    FudgeRefCount refcount1, refcount2;

    /* Construct the reference count - making sure it is initialised correctly */
    TEST_EQUALS_INT( FudgeRefCount_create ( &refcount1 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeRefCount_count ( refcount1 ), 1 );

    /* Check that incrementing the refcount has the intended effect */
    FudgeRefCount_increment ( refcount1 );
    TEST_EQUALS_INT( FudgeRefCount_count ( refcount1 ), 2 );

    /* Create another reference count - this can be used to check that instances are independent */
    TEST_EQUALS_INT( FudgeRefCount_create ( &refcount2 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeRefCount_count ( refcount2 ), 1 );

    /* Decrement the first count and check the return value */
    TEST_EQUALS_INT( FudgeRefCount_decrementAndReturn ( refcount1 ), 1 );
    TEST_EQUALS_INT( FudgeRefCount_count ( refcount1 ), 1 );
    
    /* Now increment both counts repeatedly, the first twice as often as the second */
    for ( index = 0; index < 65535; ++index )
    {
        FudgeRefCount_increment ( refcount1 );
        if ( index % 2 )
            FudgeRefCount_increment ( refcount2 );
    }
    TEST_EQUALS_INT( FudgeRefCount_count ( refcount1 ), 65536 );
    TEST_EQUALS_INT( FudgeRefCount_count ( refcount2 ), 32768 );

    /* Decrement the first count down to zero and destroy it */
    while ( ( index = FudgeRefCount_count ( refcount1 ) ) )
        FudgeRefCount_decrementAndReturn ( refcount1 );
    TEST_EQUALS_INT( FudgeRefCount_count ( refcount1 ), 0 );
    TEST_EQUALS_INT( FudgeRefCount_destroy ( refcount1 ), FUDGE_OK );

    /* Make sure that the second count is still at its previous level, then destroy that */
    TEST_EQUALS_INT( FudgeRefCount_decrementAndReturn ( refcount2 ), 32767 ); 
    TEST_EQUALS_INT( FudgeRefCount_destroy ( refcount2 ), FUDGE_OK );
END_TEST
#endif /* ifndef EXTERNAL_TESTS_ONLY */

DEFINE_TEST( StringPool )
    static const char * testStrings [ 16 ] = { "Zero",  "One",  "Two",   "Three", "Four",    "Five",  "Six",  "Seven",
                                               "Eight", "Nine", "Alpha", "Bravo", "Charlie", "Delta", "Echo", "Foxtrot" };

    FudgeStringPool pool1, pool2;
    FudgeString strings [ 16 ];
    FudgeString string1, string2;
    FudgeStatus status;
    int index;

    /* Construct the first string pool */
    TEST_EQUALS_INT( FudgeStringPool_create ( &pool1 ), FUDGE_OK );
        
    /* Construct a full set of test strings - all members of pool1 */
    for ( index = 0; index < 16; ++index )
    {
        if ( index % 2 )
            strings [ index ] = FudgeStringPool_createStringFromASCIIZ( pool1, &status, testStrings [ index ] );
        else
            strings [ index ] = FudgeStringPool_createStringFromASCII ( pool1, &status, testStrings [ index ], strlen ( testStrings [ index ] ) );
        TEST_EQUALS_INT( status, FUDGE_OK );
    }

    /* Attempt various failure cases */
    TEST_EQUALS_INT( FudgeStringPool_create ( 0 ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeStringPool_release ( 0 ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeStringPool_retain ( 0 ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeStringPool_acquire ( 0, strings [ 0 ] ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeStringPool_acquire ( pool1, 0 ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeStringPool_clear ( 0 ), FUDGE_NULL_POINTER );

    /* Construct the second pool and immediately increase its reference count */
    TEST_EQUALS_INT( FudgeStringPool_create ( &pool2 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeStringPool_retain ( pool2 ), FUDGE_OK );

    /* Add the first eight strings to the second pool, and again to the first
       pool */
    for ( index = 0; index < 8; ++index )
    {
        TEST_EQUALS_INT( FudgeString_retain ( strings [ index ] ), FUDGE_OK );
        TEST_EQUALS_INT( FudgeStringPool_acquire ( pool1, strings [ index ] ), FUDGE_OK );

        TEST_EQUALS_INT( FudgeString_retain ( strings [ index ] ), FUDGE_OK );
        TEST_EQUALS_INT( FudgeStringPool_acquire ( pool2, strings [ index ] ), FUDGE_OK );
    }

    /* Create a couple of strings - using the ASCII utility methods, but
       without providing a status variable. Grab a local reference to the
       strings. */
    TEST_EQUALS_TRUE( ( string1 = FudgeStringPool_createStringFromASCII ( pool1, 0, "", 0 ) ) != 0 );
    TEST_EQUALS_INT ( FudgeString_getSize ( string1 ), 0 );
    TEST_EQUALS_INT( FudgeString_retain ( string1 ), FUDGE_OK );

    TEST_EQUALS_TRUE( ( string2 = FudgeStringPool_createStringFromASCIIZ ( pool2, 0, "A String" ) ) != 0 );
    TEST_EQUALS_INT ( FudgeString_getSize ( string2 ), 8 );
    TEST_EQUALS_INT( FudgeString_retain ( string2 ), FUDGE_OK );

    /* Destroy the first pool and reduce the reference count of the second */
    TEST_EQUALS_INT( FudgeStringPool_release ( pool1 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeStringPool_release ( pool2 ), FUDGE_OK );

    /* The first eight strings and the empty string should still be available */
    for ( index = 0; index < 8; ++index )
        TEST_EQUALS_MEMORY( testStrings [ index ], strlen ( testStrings [ index ] ), FudgeString_getData ( strings [ index ] ), FudgeString_getSize ( strings [ index ] ) );
    TEST_EQUALS_MEMORY( 0, 0, FudgeString_getData ( string1 ), FudgeString_getSize ( string1 ) );

    /* Destroy the second pool */
    TEST_EQUALS_INT( FudgeStringPool_release ( pool2 ), FUDGE_OK );

    /* The two zero status strings should still be available */
    TEST_EQUALS_MEMORY( 0,          0, FudgeString_getData ( string1 ), FudgeString_getSize ( string1 ) );
    TEST_EQUALS_MEMORY( "A String", 8, FudgeString_getData ( string2 ), FudgeString_getSize ( string2 ) );

    /* Clean up */
    TEST_EQUALS_INT( FudgeString_release ( string1 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeString_release ( string2 ), FUDGE_OK );
END_TEST

void testFloatConversion ( float input );
void testDoubleConversion ( double input );
void testI64Conversion ( int64_t input );

DEFINE_TEST( EndianConversion )
    /* Test floats */
    testFloatConversion ( 0.0f );
#ifdef NAN
    testFloatConversion ( NAN );
#endif
    testFloatConversion ( 2147483647.0f );
    testFloatConversion ( -2147483647.0f );

    /* Test doubles */
    testDoubleConversion ( 0.0f );
    testDoubleConversion ( 9223372036854775807.0 );
    testDoubleConversion ( -9223372036854775807.0 );

    /* Test 64bit integers */
    testI64Conversion ( 0 );
    testI64Conversion ( ( int64_t ) 9223372036854775806ll );
    testI64Conversion ( ( int64_t ) -9223372036854775805ll );
END_TEST


DEFINE_TEST_SUITE( Utilities )
#ifndef EXTERNAL_TESTS_ONLY
    REGISTER_TEST( ReferenceCount )
#endif /* ifndef EXTERNAL_TESTS_ONLY */
    REGISTER_TEST( StringPool )
    REGISTER_TEST( EndianConversion )
END_TEST_SUITE


void testFloatConversion ( float input )
{
    float output, original;

    output = htonf ( input );
#ifdef _BIG_ENDIAN
    TEST_EQUALS_FLOAT( input, output, 0.0f );
#else
    TEST_EQUALS_TRUE( input == 0.0f || input != output );
#endif
    original = ntohf ( output );
    TEST_EQUALS_FLOAT( input, original, 0.0f );
}

void testDoubleConversion ( double input )
{
    double output, original;

    output = htond ( input );
#ifdef _BIG_ENDIAN
    TEST_EQUALS_FLOAT( input, output, 0.0 );
#else
    TEST_EQUALS_TRUE( input == 0.0 || input != output );
#endif
    original = ntohd ( output );
    TEST_EQUALS_FLOAT( input, original, 0.0 );
}

void testI64Conversion ( int64_t input )
{
    int64_t output, original;

    output = htoni64 ( input );
#ifdef _BIG_ENDIAN
    TEST_EQUALS_INT( input, output );
#else
    TEST_EQUALS_TRUE( input != output || ! input );
#endif
    original = ntohi64 ( output );
    TEST_EQUALS_INT( input, original );
}

