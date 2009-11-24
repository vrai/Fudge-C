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
#include "fudge/platform.h"
#include "header.h"
#include <assert.h>

#include <stdio.h>      /* TODO Remove this */

FudgeStatus FudgeCodec_decodeMsgFields ( FudgeMsg message, const fudge_byte * bytes, fudge_i32 numbytes );
FudgeStatus FudgeCodec_decodeField ( FudgeMsg message, FudgeFieldHeader header, fudge_i32 width, const fudge_byte * bytes, fudge_i32 numbytes );


FudgeStatus FudgeCodec_decodeMsg ( FudgeMsgEnvelope * envelope, const fudge_byte * bytes, fudge_i32 numbytes )
{
    FudgeStatus status;
    FudgeMsgHeader header;
    FudgeMsg message;

    if ( ! envelope )
        return FUDGE_NULL_POINTER;

    /* Get the message header and use it to create the envelope and the message */
    if ( ( status = FudgeHeader_decodeMsgHeader ( &header, bytes, numbytes ) ) != FUDGE_OK )
        return status;
    if ( numbytes < header.numbytes )
        return FUDGE_OUT_OF_BYTES;
    if ( ( status = FudgeMsg_create ( &message ) ) != FUDGE_OK )
        return status;

    envelope->directives = header.directives;
    envelope->schemaversion = header.schemaversion;
    envelope->taxonomy = header.taxonomy;
    envelope->message = message;

    /* Advance to the end of the header */
    bytes += sizeof ( FudgeMsgHeader );
    numbytes -= sizeof ( FudgeMsgHeader );

    /* Consume fields */
    if ( ( status = FudgeCodec_decodeMsgFields ( message, bytes, numbytes ) ) != FUDGE_OK )
    {
        FudgeMsg_release ( message );
        envelope->message = 0;
    }

    return status;
}


#define FUDGECODEC_TYPEDARRAYFIELD_DECODE_IMPL( type, typename, swapper )                                                                       \
    FudgeStatus FudgeCodec_decode##typename##Field ( FudgeMsg message, FudgeFieldHeader header, const type * elements, fudge_i32 numelements )  \
    {                                                                                                                                           \
        int index;                                                                                                                              \
        type * mutable;                                                                                                                         \
        FudgeStatus status;                                                                                                                     \
        if ( ! ( mutable = ( type * ) malloc ( numelements * sizeof ( type ) ) ) )                                                              \
            return FUDGE_OUT_OF_MEMORY;                                                                                                         \
        for ( index = 0; index < numelements; ++index )                                                                                         \
            mutable [ index ] = swapper ( elements [ index ] );                                                                                 \
        status = FudgeMsg_addField##typename##Array ( message, header.name, FudgeHeader_getOrdinal ( &header ), mutable, numelements );         \
        free ( mutable );                                                                                                                       \
        return status;                                                                                                                          \
    }

FUDGECODEC_TYPEDARRAYFIELD_DECODE_IMPL( fudge_i16, I16, ntohs )
FUDGECODEC_TYPEDARRAYFIELD_DECODE_IMPL( fudge_i32, I32, ntohl )
FUDGECODEC_TYPEDARRAYFIELD_DECODE_IMPL( fudge_i64, I64, ntohi64 )
FUDGECODEC_TYPEDARRAYFIELD_DECODE_IMPL( fudge_f32, F32, ntohf )
FUDGECODEC_TYPEDARRAYFIELD_DECODE_IMPL( fudge_f64, F64, ntohd )

FudgeStatus FudgeCodec_decodeField ( FudgeMsg message, FudgeFieldHeader header, fudge_i32 width, const fudge_byte * bytes, fudge_i32 numbytes )
{
    FudgeStatus status;

    if ( width > numbytes )
        return FUDGE_OUT_OF_BYTES;

    switch ( header.type )
    {
        /* Fixed width types are handled individually */
        case FUDGE_TYPE_INDICATOR:  return FudgeMsg_addFieldIndicator ( message, header.name, FudgeHeader_getOrdinal ( &header ) );
        case FUDGE_TYPE_BOOLEAN:    return FudgeMsg_addFieldBool ( message, header.name, FudgeHeader_getOrdinal ( &header ), *bytes != 0 );
        case FUDGE_TYPE_BYTE:       return FudgeMsg_addFieldByte ( message, header.name, FudgeHeader_getOrdinal ( &header ), *( ( fudge_byte * ) bytes ) );
        case FUDGE_TYPE_SHORT:      return FudgeMsg_addFieldI16  ( message, header.name, FudgeHeader_getOrdinal ( &header ), ntohs ( *( ( fudge_i16 * ) bytes ) ) );
        case FUDGE_TYPE_INT:        return FudgeMsg_addFieldI32  ( message, header.name, FudgeHeader_getOrdinal ( &header ), ntohl ( *( ( fudge_i32 * ) bytes ) ) );
        case FUDGE_TYPE_LONG:       return FudgeMsg_addFieldI64  ( message, header.name, FudgeHeader_getOrdinal ( &header ), ntohi64 ( *( ( fudge_i64 * ) bytes ) ) );
        case FUDGE_TYPE_FLOAT:      return FudgeMsg_addFieldF32  ( message, header.name, FudgeHeader_getOrdinal ( &header ), ntohf ( *( ( fudge_f32 * ) bytes ) ) );
        case FUDGE_TYPE_DOUBLE:     return FudgeMsg_addFieldF64  ( message, header.name, FudgeHeader_getOrdinal ( &header ), ntohd ( *( ( fudge_f64 * ) bytes ) ) );

        /* Submessages should be a simple list of fields */
        case FUDGE_TYPE_FUDGE_MSG:
        {
            FudgeMsg submessage;

            if ( ( status = FudgeMsg_create ( &submessage ) ) != FUDGE_OK )
                return status;
            if ( ( status = FudgeCodec_decodeMsgFields ( submessage, bytes, width ) ) == FUDGE_OK )
                status = FudgeMsg_addFieldMsg ( message, header.name, FudgeHeader_getOrdinal ( &header ), submessage );
            FudgeMsg_release ( submessage );
            return status;
        }

        /* Typed arrays need to be converted to host ordering */
        case FUDGE_TYPE_SHORT_ARRAY:
            return FudgeCodec_decodeI16Field ( message, header, ( const fudge_i16 * ) bytes, width / sizeof ( fudge_i16 ) );
        case FUDGE_TYPE_INT_ARRAY:
            return FudgeCodec_decodeI32Field ( message, header, ( const fudge_i32 * ) bytes, width / sizeof ( fudge_i32 ) );
        case FUDGE_TYPE_LONG_ARRAY:
            return FudgeCodec_decodeI64Field ( message, header, ( const fudge_i64 * ) bytes, width / sizeof ( fudge_i64 ) );
        case FUDGE_TYPE_FLOAT_ARRAY:
            return FudgeCodec_decodeF32Field ( message, header, ( const fudge_f32 * ) bytes, width / sizeof ( fudge_f32 ) );
        case FUDGE_TYPE_DOUBLE_ARRAY:
            return FudgeCodec_decodeF64Field ( message, header, ( const fudge_f64 * ) bytes, width / sizeof ( fudge_f64 ) );

        /* Everything else can be loaded as a block of bytes */
        default:
            return FudgeMsg_addFieldOpaque ( message, header.type, header.name, FudgeHeader_getOrdinal ( &header ), bytes, width );
    }
}


FudgeStatus FudgeCodec_decodeMsgFields ( FudgeMsg message, const fudge_byte * bytes, fudge_i32 numbytes )
{
    FudgeStatus status;
    FudgeFieldHeader fieldheader;

    while ( numbytes )
    {
        fudge_i32 consumed, width;

        if ( ( status = FudgeHeader_decodeFieldHeader ( &fieldheader, &consumed, bytes, numbytes ) ) != FUDGE_OK )
            return status;
        bytes += consumed;
        numbytes -= consumed;

        /* TODO Remove this */
        printf ( "\nType: %d\n", fieldheader.type );
        printf ( "Width of width: %d\n", fieldheader.widthofwidth );
        printf ( "{%s}\n", fieldheader.name ? fieldheader.name : "NULL" );
        if ( fieldheader.hasordinal )
            printf ( "Ordinal: %d\n", fieldheader.ordinal );
        printf ( "Consumed %d, %d left\n", consumed, numbytes );

        /* Get the field width */
        if ( ( status = FudgeHeader_getFieldWidth ( &width, &consumed, fieldheader, bytes, numbytes ) ) != FUDGE_OK )
            goto release_fieldheader_and_fail;
        bytes += consumed;
        numbytes -= consumed;

        /* TODO Remove this */
        printf ( "Field width: %d\n", width );
        printf ( "Consumed %d, %d left\n", consumed, numbytes );

        /* Get the field and add it to the message */
        if ( ( status = FudgeCodec_decodeField ( message, fieldheader, width, bytes, numbytes ) ) != FUDGE_OK )
            goto release_fieldheader_and_fail;
        bytes += width;
        numbytes -= width;
        printf ( "Consumed %d, %d left\n", width, numbytes );

        /* Clean up this iteration */
        FudgeHeader_destroyFieldHeader ( fieldheader );
    }
    
    return FUDGE_OK;

/* Sorry Dijkstra */
release_fieldheader_and_fail:
    FudgeHeader_destroyFieldHeader ( fieldheader );
    return status;
}

/*****************************************************************************
 * TODO Split this file about this point; put the decode and encode logic in
 * TODO separate implementation files (keep common header).
 */

FudgeStatus FudgeCodec_getMessageLength ( const FudgeMsg message, fudge_i32 * numbytes );
fudge_i32 FudgeCodec_getFieldLength ( const FudgeField * field );
fudge_i32 FudgeCodec_getFieldDataLength ( const FudgeField * field );
fudge_byte FudgeCodec_calculateBytesToHoldSize ( fudge_i32 size );

FudgeStatus FudgeCodec_encodeMsgFields ( const FudgeMsg message, fudge_byte * * writepos );
FudgeStatus FudgeCodec_encodeField ( const FudgeField * field, fudge_byte * * writepos );
fudge_byte FudgeCodec_createFieldPrefix ( const FudgeField * field );

/* TODO Rearrange this file, post split, so these aren't required */
void FudgeCodec_encodeFieldByte ( fudge_byte value, fudge_byte * * writepos );
void FudgeCodec_encodeFieldI16 ( fudge_i16 value, fudge_byte * * writepos );
void FudgeCodec_encodeFieldI32 ( fudge_i32 value, fudge_byte * * writepos );

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

    printf ( "FudgeCodec_encodeMsg: length = %d\n", *numbytes );  /* TODO Remove this */

    /* Allocate the space required for the encoded message */
    if ( ! ( *bytes = ( fudge_byte * ) malloc ( *numbytes ) ) )
        return FUDGE_OUT_OF_MEMORY;

    /* Write the message envelope */
    writepos = *bytes;
    FudgeCodec_encodeFieldByte ( envelope.directives, &writepos );
    FudgeCodec_encodeFieldByte ( envelope.schemaversion, &writepos );
    FudgeCodec_encodeFieldI16 ( envelope.taxonomy, &writepos );
    FudgeCodec_encodeFieldI32 ( *numbytes, &writepos );

    printf ( "FudgeCodec_encodeMsg: envelope moved write position %lu bytes\n", writepos - *bytes );    /* TODO Remove this */

    /* Write the top-level fields */
    if ( ( status = FudgeCodec_encodeMsgFields ( envelope.message, &writepos ) ) != FUDGE_OK )
        goto release_bytes_and_fail;

    /* Ensure that all bytes have been written */
    if ( writepos != *bytes + *numbytes )
    {
        status = FUDGE_OUT_OF_BYTES;
        goto release_bytes_and_fail;
    }

    printf ( "FudgeCodec_encodeMsg: message moved write position %lu bytes\n", writepos - *bytes );    /* TODO Remove this */
    return status;

release_bytes_and_fail:
    free ( *bytes );
    return status;
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

fudge_byte FudgeCodec_calculateBytesToHoldSize ( fudge_i32 size )
{
    if ( size < 256 )   /* Byte is considered unsigned in this instance */
        return 1;
    if ( size < 32768 ) /* Maximum value for 16 bit integer, signed in this case */
        return 2;
    return 4;
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

FudgeStatus FudgeCodec_encodeField ( const FudgeField * field, fudge_byte * * writepos )
{
    if ( ! field || ! writepos || ! *writepos )
        return FUDGE_NULL_POINTER;

    /* Encode the field header */
    FudgeCodec_encodeFieldByte ( FudgeCodec_createFieldPrefix ( field ), writepos );
    FudgeCodec_encodeFieldByte ( field->type, writepos );
    if ( field->flags & FUDGE_FIELD_HAS_ORDINAL )
        FudgeCodec_encodeFieldI16 ( field->ordinal, writepos );
    if ( field->flags & FUDGE_FIELD_HAS_NAME )
        FudgeCodec_encodeFieldOpaque ( ( const fudge_byte * ) field->name, field->name ? strlen ( field->name ) : 0, writepos );

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

fudge_byte FudgeCodec_createFieldPrefix ( const FudgeField * field )
{
    fudge_byte prefix, varwidthbytes;
    const fudge_bool isFixedWidth = FudgeType_typeIsFixedWidth ( field->type );
    
    if ( isFixedWidth )
        varwidthbytes = 0;
    else
    {
        varwidthbytes = FudgeCodec_calculateBytesToHoldSize ( FudgeCodec_getFieldDataLength ( field ) );
        assert ( varwidthbytes >= 0 && varwidthbytes <= 4 );
        if ( varwidthbytes > 2 )    /* As there's only 2 bytes for this value, 4 bytes is */
            varwidthbytes = 3;      /* stored as 3 */
    }

    prefix = varwidthbytes << 5;

    prefix |= isFixedWidth                               ? 0x80 : 0;
    prefix |= field->flags & FUDGE_FIELD_HAS_ORDINAL     ? 0x10 : 0;
    prefix |= field->flags & FUDGE_FIELD_HAS_NAME        ? 0x08 : 0;

    printf ( "Prefix: %d : %d, %d\n", field->type, varwidthbytes, ( unsigned char ) prefix );  /* TODO Remove this */

    return prefix;
}

