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
#include "fudge/string.h"
#include "codec_encode.h"
#include "header.h"
#include "message_internal.h"
#include "prefix.h"
#include "registry_internal.h"
#include <assert.h>

/* The recursive nature of FudgeMsg encoding means these have to be forward
   declared. */
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
    const FudgeTypeDesc * typedesc = FudgeRegistry_getTypeDesc ( field->type );
    
    /* Fixed width is the easiest to check */
    if ( typedesc->fixedwidth >= 0 )
        return typedesc->fixedwidth;

    /* Variable width fields are either messages, handled separately, or simply blocks
       of bytes */
    if ( typedesc->payload == FUDGE_TYPE_PAYLOAD_SUBMSG )
    {
        /* Message fields don't store their width in the field object (as
           they are mutable), so determine the width now */
        fudge_i32 fieldwidth;
        FudgeStatus status = FudgeCodec_getMessageLength ( field->data.message, &fieldwidth );
        assert ( status == FUDGE_OK );
        return fieldwidth;
    }
    else
        return field->numbytes;
}

fudge_i32 FudgeCodec_getFieldLength ( const FudgeField * field )
{
    fudge_i32 fieldwidth,
              numbytes = 2;     /* Prefix + type */

    assert ( field );

    /* Add the optional field header elements */
    if ( field->flags & FUDGE_FIELD_HAS_NAME )
        numbytes += ( field->name ? FudgeString_getSize ( field->name ) : 0 ) + 1;  /* 1 byte used for length */
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

    /* Has the length been cached? */
    if ( ( *numbytes = FudgeMsg_getWidth ( message ) ) >= 0 )
        return FUDGE_OK;

    /* Iterate over the fields in the message and sum their encoded length */
    *numbytes = 0;
    for ( index = 0, numfields = FudgeMsg_numFields ( message ); index < numfields; ++index )
        if ( ( status = FudgeMsg_getFieldAtIndex ( &field, message, index ) ) != FUDGE_OK )
            return status;
        else
            *numbytes += FudgeCodec_getFieldLength ( &field );

    /* Cache the length */
    FudgeMsg_setWidth ( message, *numbytes );

    return FUDGE_OK;
}

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
        header->namelen = field->name ? FudgeString_getSize ( field->name ) : 0;

        /* Allocate the space for the field name: as a NULL pointer indicates
           no name, allocate a single byte if the field name is NULL (used to
           indicate a field with a name of zero bytes in length). */
        if ( ! ( header->name = ( fudge_byte * ) malloc ( field->name ? header->namelen : 1 ) ) )
            return FUDGE_OUT_OF_MEMORY;

        if ( field->name )
        {
            /* Field name is non-null, copy it across in to the header */
            memcpy ( header->name, FudgeString_getData ( field->name ), header->namelen );
        }
        else
        {
            /* Field name is null, set the placeholder byte to be NULL and
               indicate the true length of the name in the length field */
            *( header->name ) = 0;
        }
    }
    else
    {
        /* No name: zero the fields to indicate this */
        header->name = 0;
        header->namelen = 0;
    }

    return FUDGE_OK;
}

FudgeStatus FudgeCodec_encodeField ( const FudgeField * field, fudge_byte * * writepos )
{
    FudgeFieldHeader header;
    FudgeStatus status;
    FudgeTypeEncoder encoder;
    const FudgeTypeDesc * typedesc = FudgeRegistry_getTypeDesc ( field->type );

    if ( ! field || ! writepos || ! *writepos )
        return FUDGE_NULL_POINTER;

    /* Populate and encode the header */
    if ( ( status = FudgeCodec_populateFieldHeader ( field, &header ) ) != FUDGE_OK )
        return status;
    status = FudgeHeader_encodeFieldHeader ( &header, writepos );
    FudgeHeader_destroyFieldHeader ( header );
    if ( status != FUDGE_OK )
        return status;

    /* If available for this type, use the registered encoder. Failing that,
       treat it as an array of bytes. */
    encoder = typedesc->encoder ? typedesc->encoder
                                : FudgeCodec_encodeFieldByteArray;

    return encoder ( field, writepos );
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

FudgeStatus FudgeCodec_encodeFieldIndicator ( const FudgeField * field, fudge_byte * * data )
{
    return FUDGE_OK;
}

FudgeStatus FudgeCodec_encodeFieldBool ( const FudgeField * field, fudge_byte * * data )
{
    FudgeCodec_encodeByte ( field->data.boolean, data );
    return FUDGE_OK;
}

#define FUDGECODEC_ENCODE_FIELD_IMPL( typename, slot )                                              \
    FudgeStatus FudgeCodec_encodeField##typename ( const FudgeField * field, fudge_byte * * data )  \
    {                                                                                               \
        FudgeCodec_encode##typename ( field->data.slot, data );                                     \
        return FUDGE_OK;                                                                            \
    }

FUDGECODEC_ENCODE_FIELD_IMPL( Byte,   byte )
FUDGECODEC_ENCODE_FIELD_IMPL( I16,    i16 )
FUDGECODEC_ENCODE_FIELD_IMPL( I32,    i32 )
FUDGECODEC_ENCODE_FIELD_IMPL( I64,    i64 )
FUDGECODEC_ENCODE_FIELD_IMPL( F32,    f32 )
FUDGECODEC_ENCODE_FIELD_IMPL( F64,    f64 )
FUDGECODEC_ENCODE_FIELD_IMPL( String, string )

#define FUDGECODEC_ENCODE_ARRAY_IMPL( typename, type )                                                      \
    FudgeStatus FudgeCodec_encodeField##typename##Array ( const FudgeField * field, fudge_byte * * data )   \
    {                                                                                                       \
        fudge_i32 index;                                                                                    \
        const fudge_i32 numelements = field->numbytes / sizeof ( type );                                    \
        const type * elements = ( const type * ) field->data.bytes;                                         \
                                                                                                            \
        FudgeCodec_encodeFieldLength ( field->numbytes, data );                                             \
        for ( index = 0; index < numelements; ++index )                                                     \
            FudgeCodec_encode##typename ( elements [ index ], data );                                       \
        return FUDGE_OK;                                                                                    \
    }

FUDGECODEC_ENCODE_ARRAY_IMPL( I16, fudge_i16 )
FUDGECODEC_ENCODE_ARRAY_IMPL( I32, fudge_i32 )
FUDGECODEC_ENCODE_ARRAY_IMPL( I64, fudge_i64 )
FUDGECODEC_ENCODE_ARRAY_IMPL( F32, fudge_f32 )
FUDGECODEC_ENCODE_ARRAY_IMPL( F64, fudge_f64 )

FudgeStatus FudgeCodec_encodeFieldFudgeMsg ( const FudgeField * field, fudge_byte * * data )
{
    FudgeStatus status;
    fudge_i32 numbytes;

    if ( ( status = FudgeCodec_getMessageLength ( field->data.message, &numbytes ) ) != FUDGE_OK )
        return status;
    FudgeCodec_encodeFieldLength ( numbytes, data );
    return FudgeCodec_encodeMsgFields ( field->data.message, data );
}

/*****************************************************************************
 * Functions from fudge/codec.h
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
    FudgeCodec_encodeByte ( envelope.directives, &writepos );
    FudgeCodec_encodeByte ( envelope.schemaversion, &writepos );
    FudgeCodec_encodeI16 ( envelope.taxonomy, &writepos );
    FudgeCodec_encodeI32 ( *numbytes, &writepos );

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

/*****************************************************************************
 * Functions from fudge/codec_ex.h
 */

void FudgeCodec_encodeFieldLength ( const fudge_i32 length, fudge_byte * * data )
{
    switch ( FudgeCodec_calculateBytesToHoldSize ( length ) )
    {
        case 0:                                            break;
        case 1:  FudgeCodec_encodeByte ( (const fudge_byte) length, data );   break;
        case 2:  FudgeCodec_encodeI16 ( (const fudge_i16) length, data );    break;   
        default: FudgeCodec_encodeI32 ( length, data );    break;
    }
}

void FudgeCodec_encodeByte ( const fudge_byte byte, fudge_byte * * data )
{
    **data = byte;
    ( *data ) += 1;
}

#define FUDGECODEC_ENCODE_TYPE_IMPL( typename, type, swapper )                 \
    void FudgeCodec_encode##typename ( type value, fudge_byte * * writepos )   \
    {                                                                          \
        value = swapper ( value );                                             \
        memcpy ( *writepos, &value, sizeof ( type ) );                         \
        *writepos += sizeof ( type );                                          \
    }

FUDGECODEC_ENCODE_TYPE_IMPL( I16, fudge_i16, htons )
FUDGECODEC_ENCODE_TYPE_IMPL( I32, fudge_i32, htonl )
FUDGECODEC_ENCODE_TYPE_IMPL( I64, fudge_i64, htoni64 )
FUDGECODEC_ENCODE_TYPE_IMPL( F32, fudge_f32, htonf )
FUDGECODEC_ENCODE_TYPE_IMPL( F64, fudge_f64, htond )

void FudgeCodec_encodeString ( const FudgeString string, fudge_byte * * data )
{
    const size_t size = FudgeString_getSize ( string );
    const fudge_byte * bytes = FudgeString_getData ( string );

    FudgeCodec_encodeFieldLength ( size, data );
    if ( bytes && size )
        memcpy ( *data, bytes, size );
    ( *data ) += size;
}

void FudgeCodec_encodeByteArray ( const fudge_byte * bytes,
                                  const fudge_i32 width,
                                  const fudge_bool fixedwidth,
                                  fudge_byte * * data )
{
    if ( ! fixedwidth )
        FudgeCodec_encodeFieldLength ( width, data );

    if ( bytes && data > 0 )
        memcpy ( *data, bytes, width );
    ( *data ) += width;
}

FudgeStatus FudgeCodec_encodeFieldByteArray ( const FudgeField * field, fudge_byte * * data )
{
    const FudgeTypeDesc * typedesc = FudgeRegistry_getTypeDesc ( field->type );

    FudgeCodec_encodeByteArray ( field->data.bytes,
                                 field->numbytes,
                                 typedesc->fixedwidth >= 0,
                                 data );
    return FUDGE_OK;
}

