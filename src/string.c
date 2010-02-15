/**
 * Copyright (C) 2010 - 2010, Vrai Stacey.
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
#include "fudge/string.h"
#include "convertutf.h"
#include "reference.h"

struct FudgeStringImpl
{
    FudgeRefCount refcount;
    fudge_byte * bytes;
    size_t numbytes;
};

FudgeStatus FudgeString_convertUTFResultToStatus ( ConversionResult result )
{
    switch ( result )
    {
        case conversionOK:      return FUDGE_OK;
        case sourceExhausted:   return FUDGE_STRING_INCOMPLETE_UNICODE;
        default:                return FUDGE_STRING_INVALID_UNICODE;
    }
}

FudgeStatus FudgeString_allocate ( FudgeString * string, size_t numbytes )
{
    FudgeStatus status;

    if ( ! string )
        return FUDGE_NULL_POINTER;

    if ( ! ( *string = ( FudgeString ) malloc ( sizeof ( struct FudgeStringImpl ) ) ) )
        return FUDGE_OUT_OF_MEMORY;

    if ( ( status = FudgeRefCount_create ( &( ( *string )->refcount ) ) ) != FUDGE_OK )
        goto free_string_and_fail;

    if ( numbytes )
    {
        if ( ! ( ( *string )->bytes = ( fudge_byte * ) malloc ( numbytes ) ) )
        {
            status = FUDGE_OUT_OF_MEMORY;
            goto destroy_refcount_and_fail;
        }
    }
    else
        ( *string )->bytes = 0;

    return FUDGE_OK;

destroy_refcount_and_fail:
    FudgeRefCount_destroy ( ( *string )->refcount );

free_string_and_fail:
    free ( string );
    return status;
}

void FudgeString_destroy ( FudgeString string )
{
    if ( string )
    {
        FudgeRefCount_destroy ( string->refcount );
        free ( string->bytes );
        free ( string );
    }
}

FudgeStatus FudgeString_createFromASCII ( FudgeString * string, const char * chars, size_t numchars )
{
    FudgeStatus status;
    size_t index;

    /* NULL character arrays are accepted, but only with a length of zero */
    if ( ( ! chars ) && numchars )
        return FUDGE_NULL_POINTER;

    if ( ( status = FudgeString_allocate ( string, numchars ) ) != FUDGE_OK )
        return status;

    for ( index = 0; index < numchars; ++index )
    {
        if ( chars [ index ] >= 0 )
            ( *string )->bytes [ index ] = chars [ index ];
        else
        {
            FudgeString_destroy ( *string );
            return FUDGE_STRING_INVALID_ASCII;
        }
    }
    ( *string )->numbytes = numchars;
    return FUDGE_OK;
}

FudgeStatus FudgeString_createFromASCIIZ ( FudgeString * string, const char * chars )
{
    return FudgeString_createFromASCII ( string, chars, chars ? strlen ( chars ) : 0u );
}

FudgeStatus FudgeString_createFromUTF8 ( FudgeString * string, const fudge_byte * bytes, size_t numbytes )
{
    FudgeStatus status;

    if ( ( ! bytes ) && numbytes )
        return FUDGE_NULL_POINTER;

    if ( numbytes && ! isLegalUTF8Sequence ( ( const UTF8 * ) bytes, ( const UTF8 * ) bytes + numbytes ) )
        return FUDGE_STRING_INVALID_UNICODE;

    if ( ( status = FudgeString_allocate ( string, numbytes ) ) != FUDGE_OK )
        return status;

    if ( numbytes )
        memcpy ( ( *string )->bytes, bytes, numbytes );
    else
        ( *string )->bytes = 0;

    ( *string )->numbytes = numbytes;
    return FUDGE_OK;
}

FudgeStatus FudgeString_createFromUTF16 ( FudgeString * string, const fudge_byte * bytes, size_t numbytes )
{
    FudgeStatus status;

    if ( ( ! bytes ) && numbytes )
        return FUDGE_NULL_POINTER;

    if ( ( status = FudgeString_allocate ( string, numbytes * 2 ) ) != FUDGE_OK )
        return status;

    if ( numbytes )
    {
        ConversionResult result;
        const UTF16 * sourceStart = ( const UTF16 * ) bytes;
        UTF8 * targetStart = ( UTF8 * ) ( *string )->bytes;

        if ( ( result = ConvertUTF16toUTF8 ( &sourceStart,
                                             ( const UTF16 * ) ( bytes + numbytes ),
                                             &targetStart,
                                             ( UTF8 * ) ( ( *string )->bytes + numbytes * 2 ),
                                             lenientConversion ) ) )
        {
            status = FudgeString_convertUTFResultToStatus ( result );
            goto destroy_string_and_fail;
        }
        ( *string )->numbytes = ( size_t ) ( targetStart - ( const UTF8 * ) ( ( *string )->bytes ) );
    }
    else
        ( *string )->numbytes = 0;
    return FUDGE_OK;

destroy_string_and_fail:
    FudgeString_destroy ( *string );
    return status;
}

FudgeStatus FudgeString_createFromUTF32 ( FudgeString * string, const fudge_byte * bytes, size_t numbytes )
{
    FudgeStatus status;

    if ( ( ! bytes ) && numbytes )
        return FUDGE_NULL_POINTER;

    if ( ( status = FudgeString_allocate ( string, numbytes ) ) != FUDGE_OK )
        return status;

    if ( numbytes )
    {
        ConversionResult result;
        const UTF32 * sourceStart = ( const UTF32 * ) bytes;
        UTF8 * targetStart = ( UTF8 * ) ( *string )->bytes;

        if ( ( result = ConvertUTF32toUTF8 ( &sourceStart,
                                             ( const UTF32 * ) ( bytes + numbytes ),
                                             &targetStart,
                                             ( UTF8 * ) ( ( *string )->bytes + numbytes ),
                                             lenientConversion ) ) )
        {
            status = FudgeString_convertUTFResultToStatus ( result );
            goto destroy_string_and_fail;
        }
        ( *string )->numbytes = ( size_t ) ( targetStart - ( const UTF8 * ) ( ( *string )->bytes ) );
    }
    else
        ( *string )->numbytes = 0;
    return FUDGE_OK;

destroy_string_and_fail:
    FudgeString_destroy ( *string );
    return status;
}

FudgeStatus FudgeString_retain ( FudgeString string )
{
    if ( ! string )
        return FUDGE_NULL_POINTER;

    FudgeRefCount_increment ( string->refcount );
    return FUDGE_OK;
}

FudgeStatus FudgeString_release ( FudgeString string )
{
    if ( ! string )
        return FUDGE_NULL_POINTER;

    if ( ! FudgeRefCount_decrementAndReturn ( string->refcount ) )
        FudgeString_destroy ( string );
    return FUDGE_OK;
}

size_t FudgeString_getSize ( const FudgeString string )
{
    return string ? string->numbytes : 0;
}

const fudge_byte * FudgeString_getData ( const FudgeString string )
{
    return string ? string->bytes : 0;
}

FudgeStatus FudgeString_convertToASCIIZ ( char * * target, const FudgeString string )
{
    if ( ! ( target && string ) )
        return FUDGE_NULL_POINTER;

    if ( ! ( *target = ( char * ) malloc ( string->numbytes + 1 ) ) )
        return FUDGE_OUT_OF_MEMORY;

    if ( string->numbytes )
    {
        size_t readPosition = 0,
               writePosition = 0,
               trailing;
        char character;
        while ( readPosition < string->numbytes )
        {
            character = string->bytes [ readPosition++ ];

            if ( ( trailing = trailingBytesForUTF8 [ ( UTF8 ) character ] ) )
            {
                /* Unicode character is the start of a sequence: make sure
                   there's enough bytes in the source to complete it */
                if ( readPosition + trailing > string->numbytes )
                {
                    free ( *target );
                    return FUDGE_STRING_INCOMPLETE_UNICODE;
                }

                /* UTF8 strings can contain BOM sequences (0xefbbbf), these
                   should be skipped over */
                if ( trailing == 2 && ( UTF8 ) character == 0xef
                                   && ( UTF8 ) ( string->bytes [ readPosition ] ) == 0xbb
                                   && ( UTF8 ) ( string->bytes [ readPosition + 1 ] ) == 0xbf )
                {
                    readPosition += trailing;
                    continue;
                }

                character = '?';
                readPosition += trailing;
            }
            else
            {
                if ( character < 1 || character > 126 )
                    character = '?';
            }

            ( *target ) [ writePosition++ ] = character;
        }
        ( *target ) [ writePosition ] = 0;
    }
    else
        **target = 0;

    return FUDGE_OK;
}

FudgeStatus FudgeString_convertToUTF16 ( fudge_byte * * target, size_t * numbytes, const FudgeString string )
{
    ConversionResult result;
    const UTF8 * sourceStart;
    UTF16 * targetStart;

    if ( ! ( target && numbytes && string ) )
        return FUDGE_NULL_POINTER;

    if ( ! string->numbytes )
    {
        *numbytes = 0;
        *target = 0;
        return FUDGE_OK;
    }

    if ( ! ( *target = ( fudge_byte * ) malloc ( string->numbytes * 2 ) ) )
        return FUDGE_OUT_OF_MEMORY;

    sourceStart = ( const UTF8 * ) string->bytes;
    targetStart = ( UTF16 * ) *target;

    if ( ( result = ConvertUTF8toUTF16 ( &sourceStart,
                                         ( const UTF8 * ) ( sourceStart + string->numbytes ),
                                         &targetStart,
                                         ( UTF16 * ) ( *target + string->numbytes * 2 ),
                                         lenientConversion ) ) )
    {
        free ( *target );
        return FudgeString_convertUTFResultToStatus ( result );
    }
    *numbytes = ( size_t ) targetStart - ( size_t ) *target;
    return FUDGE_OK;
}

FudgeStatus FudgeString_convertToUTF32 ( fudge_byte * * target, size_t * numbytes, const FudgeString string )
{
    ConversionResult result;
    const UTF8 * sourceStart;
    UTF32 * targetStart;

    if ( ! ( target && numbytes && string ) )
        return FUDGE_NULL_POINTER;

    if ( ! string->numbytes )
    {
        *numbytes = 0;
        *target = 0;
        return FUDGE_OK;
    }

    if ( ! ( *target = ( fudge_byte * ) malloc ( string->numbytes * 4 ) ) )
        return FUDGE_OUT_OF_MEMORY;

    sourceStart = ( const UTF8 * ) string->bytes;
    targetStart = ( UTF32 * ) *target;

    if ( ( result = ConvertUTF8toUTF32 ( &sourceStart,
                                         ( const UTF8 * ) ( sourceStart + string->numbytes ),
                                         &targetStart,
                                         ( UTF32 * ) ( *target + string->numbytes * 4 ),
                                         lenientConversion ) ) )
    {
        free ( *target );
        return FudgeString_convertUTFResultToStatus ( result );
    }
    *numbytes = ( size_t ) targetStart - ( size_t ) *target;
    return FUDGE_OK;
}

inline int FudgeString_byteCompare ( UTF8 left, UTF8 right )
{
    if ( left == right )
        return 0;
    else
        return left > right ? 1 : -1;
}

inline size_t FudgeString_skipBOM ( const fudge_byte * bytes, size_t position, size_t size )
{
    if ( size - position > 2 &&
         ( UTF8 ) bytes [ position ] == 0xef &&
         ( UTF8 ) bytes [ position + 1 ] == 0xbb &&
         ( UTF8 ) bytes [ position + 2 ] == 0xbf )
        return position + 3;
    else
        return position;
}

int FudgeString_compare ( const FudgeString left, const FudgeString right )
{
    size_t leftpos, leftsize, rightpos, rightsize, newleftpos, newrightpos;
    int comparison;

    /* Handle null pointers */
    if ( ! left )
        return right ? -1 : 0;
    if ( ! right )
        return 1;

    /* Byte comparison: first differing byte counts. No byte vs any byte is low. */
    leftsize = left->numbytes;
    rightsize = right->numbytes;
    leftpos = 0;
    rightpos = 0;
    for ( ; ; )
    {
        /* Check for overruns */
        if ( leftpos >= leftsize )
            return rightpos >= rightsize ? 0 : -1;
        else if ( rightpos >= rightsize )
            return 1;

        /* Check both strings for a BOM. If one or both of them is at such a
           sequence, skip the offending bytes and jump back to the start of
           the loop (in case the BOM was the last character in the string and
           an overrun has no occured). */
        newleftpos = FudgeString_skipBOM ( left->bytes, leftpos, leftsize );
        newrightpos = FudgeString_skipBOM ( right->bytes, rightpos, rightsize );
        if ( newleftpos != leftpos || newrightpos != rightpos )
        {
            leftpos = newleftpos;
            rightpos = newrightpos;
            continue;
        }

        if ( ( comparison = FudgeString_byteCompare ( left->bytes [ leftpos ],
                                                      right->bytes [ rightpos ] ) ) )
            return comparison;

        ++leftpos;
        ++rightpos;
    }
}

