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
#include "codec_encode.h"
#include "header.h"
#include "prefix.h"
#include <assert.h>

FudgeStatus FudgeCodec_getMessageLength ( const FudgeMsg message, fudge_i32 * numbytes );
FudgeStatus FudgeCodec_encodeMsgFields ( const FudgeMsg message, fudge_byte * * writepos );


fudge_byte FudgeCodec_calculateBytesToHoldSize ( fudge_i32 size )
{
    if ( size < 256 )   /* Byte is considered unsigned in this instance */
        return 1;
    if ( size < 32768 ) /* Maximum value for 16 bit integer, signed in this case */
        return 2;
    return 4;
}

fudge_i32 FudgeCodec_getFieldDataLength ( const FudgeField * field )
{
    assert ( field );

    if ( field->type == FUDGE_TYPE_FUDGE_MSG )
    {
        /* Message fields don't store their width in the field object (as
           they are mutable), so determine the width now */
        /* TODO Memoize the width the of message fields to avoid repeated calls */
        fudge_i32 fieldwidth;
        FudgeStatus status = FudgeCodec_getMessageLength ( field->data.message, &fieldwidth );
        assert ( status == FUDGE_OK );
        return fieldwidth;
    }
    else
        return field->width;
}

fudge_i32 FudgeCodec_getFieldLength ( const FudgeField * field )
{
    fudge_i32 fieldwidth,
              numbytes = 2;     /* Prefix + type */

    assert ( field );

    /* Add the optional field header elements */
    if ( field->flags & FUDGE_FIELD_HAS_NAME )
        numbytes += ( field->name ? strlen ( field->name ) : 0 ) + 1;  /* 1 byte used for length */
    if ( field->flags & FUDGE_FIELD_HAS_ORDINAL )
        numbytes += 2;

    /* Get the width of the field width */
    numbytes += ( fieldwidth = FudgeCodec_getFieldDataLength ( field ) );

    /* For variable width fields, add the space required to hold the width */
    if ( ! FudgeType_typeIsFixedWidth ( field->type ) )
        numbytes += FudgeCodec_calculateBytesToHoldSize ( fieldwidth );

    return numbytes;
}

FudgeStatus FudgeCodec_getMessageLength ( const FudgeMsg message, fudge_i32 * numbytes )
{
    unsigned long index, numfields;
    FudgeField field;
    FudgeStatus status;

    if ( ! ( message && numbytes ) )
        return FUDGE_NULL_POINTER;

    /* Iterate over the fields in the message and sum their encoded length */
    *numbytes = 0;
    for ( index = 0, numfields = FudgeMsg_numFields ( message ); index < numfields; ++index )
        if ( ( status = FudgeMsg_getFieldAtIndex ( &field, message, index ) ) != FUDGE_OK )
            return status;
        else
            *numbytes += FudgeCodec_getFieldLength ( &field );

    return FUDGE_OK;
}

void FudgeCodec_encodeFieldByte ( fudge_byte byte, fudge_byte * * writepos )
{
    **writepos = byte;
    ( *writepos ) += 1;
}

#define FUDGECODEC_ENCODEFIELD_IMPL( typename, type, swapper )                      \
    void FudgeCodec_encodeField##typename ( type value, fudge_byte * * writepos )   \
    {                                                                               \
        value = swapper ( value );                                                  \
        memcpy ( *writepos, &value, sizeof ( type ) );                              \
        *writepos += sizeof ( type );                                               \
    }

FUDGECODEC_ENCODEFIELD_IMPL( I16, fudge_i16, htons )
FUDGECODEC_ENCODEFIELD_IMPL( I32, fudge_i32, htonl )
FUDGECODEC_ENCODEFIELD_IMPL( I64, fudge_i64, htoni64 )
FUDGECODEC_ENCODEFIELD_IMPL( F32, fudge_f32, htonf )
FUDGECODEC_ENCODEFIELD_IMPL( F64, fudge_f64, htond )

void FudgeCodec_encodeFieldFixedByteArray ( const fudge_byte * bytes, fudge_i32 length, fudge_byte * * writepos )
{
    if ( bytes && length )
        memcpy ( *writepos, bytes, length );
    ( *writepos ) += length;
}

void FudgeCodec_encodeFieldLength ( fudge_i32 length, fudge_byte * * writepos )
{
    switch ( FudgeCodec_calculateBytesToHoldSize ( length ) )
    {
        case 0:                                                     break;
        case 1:  FudgeCodec_encodeFieldByte ( length, writepos );   break;
        case 2:  FudgeCodec_encodeFieldI16 ( length, writepos );    break;   
        default: FudgeCodec_encodeFieldI32 ( length, writepos );    break;
    }
}

void FudgeCodec_encodeFieldOpaque ( const fudge_byte * bytes, fudge_i32 length, fudge_byte * * writepos )
{
    FudgeCodec_encodeFieldLength ( length, writepos );
    FudgeCodec_encodeFieldFixedByteArray ( bytes, length, writepos );
}

#define FUDGECODEC_ENCODEFIELDARRAY_IMPL( typename, type )                                                                  \
    void FudgeCodec_encodeField##typename##Array ( const fudge_byte * bytes, fudge_i32 numbytes, fudge_byte * * writepos )  \
    {                                                                                                                       \
        fudge_i32 index;                                                                                                    \
        const fudge_i32 numelements = numbytes / sizeof ( type );                                                           \
        const type * elements = ( const type * ) bytes;                                                                     \
        FudgeCodec_encodeFieldLength ( numbytes, writepos );                                                                \
        for ( index = 0; index < numelements; ++index )                                                                     \
            FudgeCodec_encodeField##typename ( elements [ index ], writepos );                                              \
    }

FUDGECODEC_ENCODEFIELDARRAY_IMPL( I16, fudge_i16 )
FUDGECODEC_ENCODEFIELDARRAY_IMPL( I32, fudge_i32 )
FUDGECODEC_ENCODEFIELDARRAY_IMPL( I64, fudge_i64 )
FUDGECODEC_ENCODEFIELDARRAY_IMPL( F32, fudge_f32 )
FUDGECODEC_ENCODEFIELDARRAY_IMPL( F64, fudge_f64 )

FudgeStatus FudgeCodec_populateFieldHeader ( const FudgeField * field, FudgeFieldHeader * header )
{
    if ( ! ( field && header ) )
        return FUDGE_NULL_POINTER;

    header->type = field->type;
    if ( FudgeType_typeIsFixedWidth ( field->type ) )
        header->widthofwidth = 0;
    else
        header->widthofwidth = FudgeCodec_calculateBytesToHoldSize ( FudgeCodec_getFieldDataLength ( field ) );;

    header->hasordinal = field->flags & FUDGE_FIELD_HAS_ORDINAL;
    header->ordinal = header->hasordinal ? field->ordinal : 0;

    if ( field->flags & FUDGE_FIELD_HAS_NAME )
    {
        if ( ! ( header->name = field->name ? strdup ( field->name ) : strdup ( "" ) ) )
            return FUDGE_OUT_OF_MEMORY;
    }
    else
        header->name = 0;

    return FUDGE_OK;
}

FudgeStatus FudgeCodec_encodeField ( const FudgeField * field, fudge_byte * * writepos )
{
    FudgeFieldHeader header;
    FudgeStatus status;

    if ( ! field || ! writepos || ! *writepos )
        return FUDGE_NULL_POINTER;

    /* Populate and encode the header */
    if ( ( status = FudgeCodec_populateFieldHeader ( field, &header ) ) != FUDGE_OK )
        return status;
    status = FudgeHeader_encodeFieldHeader ( &header, writepos );
    FudgeHeader_destroyFieldHeader ( header );
    if ( status != FUDGE_OK )
        return status;

    /* Encode the field data */
    switch ( field->type )
    {
        /* Zero length fields */
        case FUDGE_TYPE_INDICATOR:  break;

        /* Fixed length, single-byte primitives */
        case FUDGE_TYPE_BOOLEAN:    FudgeCodec_encodeFieldByte ( field->data.byte, writepos );      break;
        case FUDGE_TYPE_BYTE:       FudgeCodec_encodeFieldByte ( field->data.boolean, writepos );   break;

        /* Fixed length, multi-byte primitives */
        case FUDGE_TYPE_SHORT:      FudgeCodec_encodeFieldI16 ( field->data.i16, writepos );    break;
        case FUDGE_TYPE_INT:        FudgeCodec_encodeFieldI32 ( field->data.i32, writepos );    break;
        case FUDGE_TYPE_LONG:       FudgeCodec_encodeFieldI64 ( field->data.i64, writepos );    break;
        case FUDGE_TYPE_FLOAT:      FudgeCodec_encodeFieldF32 ( field->data.f32, writepos );    break;
        case FUDGE_TYPE_DOUBLE:     FudgeCodec_encodeFieldF64 ( field->data.f64, writepos );    break;

        /* Fixed length byte arrays */
        case FUDGE_TYPE_BYTE_ARRAY_4:
        case FUDGE_TYPE_BYTE_ARRAY_8:
        case FUDGE_TYPE_BYTE_ARRAY_16:
        case FUDGE_TYPE_BYTE_ARRAY_20:
        case FUDGE_TYPE_BYTE_ARRAY_32:
        case FUDGE_TYPE_BYTE_ARRAY_64:
        case FUDGE_TYPE_BYTE_ARRAY_128:
        case FUDGE_TYPE_BYTE_ARRAY_256:
        case FUDGE_TYPE_BYTE_ARRAY_512:
            FudgeCodec_encodeFieldFixedByteArray ( field->data.bytes, field->numbytes, writepos );
            break;

        /* Variable length typed arrays */
        case FUDGE_TYPE_SHORT_ARRAY:    FudgeCodec_encodeFieldI16Array ( field->data.bytes, field->numbytes, writepos );    break;
        case FUDGE_TYPE_INT_ARRAY:      FudgeCodec_encodeFieldI32Array ( field->data.bytes, field->numbytes, writepos );    break;
        case FUDGE_TYPE_LONG_ARRAY:     FudgeCodec_encodeFieldI64Array ( field->data.bytes, field->numbytes, writepos );    break;
        case FUDGE_TYPE_FLOAT_ARRAY:    FudgeCodec_encodeFieldF32Array ( field->data.bytes, field->numbytes, writepos );    break;
        case FUDGE_TYPE_DOUBLE_ARRAY:   FudgeCodec_encodeFieldF64Array ( field->data.bytes, field->numbytes, writepos );    break;

        /* Message */
        case FUDGE_TYPE_FUDGE_MSG:
        {
            FudgeStatus status;
            fudge_i32 numbytes;

            /* TODO This should be cached in a context object! */
            if ( ( status = FudgeCodec_getMessageLength ( field->data.message, &numbytes ) ) != FUDGE_OK )
                return status;
            FudgeCodec_encodeFieldLength ( numbytes, writepos );
            if ( ( status = FudgeCodec_encodeMsgFields ( field->data.message, writepos ) ) != FUDGE_OK )
                return status;
            break;
        }

        /* Variable length byte arrays and opaque types */
        case FUDGE_TYPE_STRING:
        case FUDGE_TYPE_BYTE_ARRAY:
        default:
            FudgeCodec_encodeFieldOpaque ( field->data.bytes, field->numbytes, writepos );
            break;
    }

    return FUDGE_OK;
}

FudgeStatus FudgeCodec_encodeMsgFields ( const FudgeMsg message, fudge_byte * * writepos )
{
    FudgeStatus status;
    FudgeField field;
    unsigned long index, numfields;

    if ( ! writepos || ! writepos || ! *writepos )
        return FUDGE_NULL_POINTER;

    for ( index = 0, numfields = FudgeMsg_numFields ( message ); index < numfields; ++index )
        if ( ( status = FudgeMsg_getFieldAtIndex ( &field, message, index ) ) != FUDGE_OK )
            return status;
        else
            if ( ( status = FudgeCodec_encodeField ( &field, writepos ) ) != FUDGE_OK )
                return status;

    return FUDGE_OK;
}

/******************************************************************************
 * Externally accessible functions
 */

FudgeStatus FudgeCodec_encodeMsg ( FudgeMsgEnvelope envelope, fudge_byte * * bytes, fudge_i32 * numbytes )
{
    FudgeStatus status;
    fudge_byte * writepos;

    if ( ! ( bytes && numbytes && envelope.message ) )
        return FUDGE_NULL_POINTER;

    /* Get the length of the message, plus the envelope header */
    if ( ( status = FudgeCodec_getMessageLength ( envelope.message, numbytes ) ) != FUDGE_OK )
        return status;
    *numbytes += sizeof ( FudgeMsgHeader );

    /* Allocate the space required for the encoded message */
    if ( ! ( *bytes = ( fudge_byte * ) malloc ( *numbytes ) ) )
        return FUDGE_OUT_OF_MEMORY;

    /* Write the message envelope */
    writepos = *bytes;
    FudgeCodec_encodeFieldByte ( envelope.directives, &writepos );
    FudgeCodec_encodeFieldByte ( envelope.schemaversion, &writepos );
    FudgeCodec_encodeFieldI16 ( envelope.taxonomy, &writepos );
    FudgeCodec_encodeFieldI32 ( *numbytes, &writepos );

    /* Write the top-level fields */
    if ( ( status = FudgeCodec_encodeMsgFields ( envelope.message, &writepos ) ) != FUDGE_OK )
        goto release_bytes_and_fail;

    /* Ensure that all bytes have been written */
    if ( writepos != *bytes + *numbytes )
    {
        status = FUDGE_OUT_OF_BYTES;
        goto release_bytes_and_fail;
    }

    return status;

release_bytes_and_fail:
    free ( *bytes );
    return status;
}

