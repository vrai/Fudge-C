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
#include "reference.h"
#include "simpletest.h"

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


void testFloatConversion ( float input );
void testDoubleConversion ( double input );
void testI64Conversion ( int64_t input );

DEFINE_TEST( EndianConversion )
    /* Test floats */
    testFloatConversion ( 0.0f );
#ifdef NAN
    testFloatConversion ( NAN );
#endif
    testFloatConversion ( 1E31 );
    testFloatConversion ( -1E31 );

    /* Test doubles */
    testDoubleConversion ( 0.0f );
    testDoubleConversion ( 1E63 );
    testDoubleConversion ( -1E63 );

    /* Test 64bit integers */
    testI64Conversion ( 0 );
    testI64Conversion ( ( int64_t ) 2E31 );
    testI64Conversion ( ( int64_t ) -2E31 );
END_TEST


DEFINE_TEST_SUITE( Utilities )
    REGISTER_TEST( ReferenceCount )
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

