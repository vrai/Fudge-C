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

#define ALLNAMES_FILENAME "test_data/allNames.dat"

void loadFile ( fudge_byte * * target, fudge_i32 * targetSize, const char * filename );
FudgeStatus appendBytes ( fudge_byte * * target, fudge_i32 * targetSize, fudge_byte * source, fudge_i32 sourceSize );

DEFINE_TEST( DecodeAllNames )
    fudge_byte * referenceBytes;
    fudge_i32 numReferenceBytes;
    FudgeMsg message;

    loadFile ( &referenceBytes, &numReferenceBytes, ALLNAMES_FILENAME );

    TEST_EQUALS_INT( FudgeCodec_decodeMsg ( &message, referenceBytes, numReferenceBytes ), FUDGE_OK );

    /* TODO Check message contents */

    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );
END_TEST


DEFINE_TEST_SUITE( Codec )
    REGISTER_TEST( DecodeAllNames )
END_TEST_SUITE


void loadFile ( fudge_byte * * target, fudge_i32 * targetSize, const char * filename )
{
    FILE * file;
    fudge_byte buffer [ 1024 ];

    TEST_EQUALS_TRUE( ( file = fopen ( filename, "r" ) ) != 0 );
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

