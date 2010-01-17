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
#include "codec_decode.h"
#include "header.h"
#include "registry_internal.h"
#include <assert.h>

fudge_i32 FudgeCodec_getNumBytes ( const FudgeTypeDesc * typedesc, fudge_i32 width )
{
    if ( typedesc->payload == FUDGE_TYPE_PAYLOAD_BYTES )
        return typedesc->fixedwidth >= 0 ? typedesc->fixedwidth : width;
    else
        return 0;
}

FudgeStatus FudgeCodec_decodeField ( FudgeMsg message, FudgeFieldHeader header, fudge_i32 width, const fudge_byte * bytes, fudge_i32 numbytes )
{
    FudgeStatus status;
    FudgeTypeDecoder decoder;
    FudgeFieldData data;
    const FudgeTypeDesc * typedesc = FudgeRegistry_getTypeDesc ( header.type );

    if ( width > numbytes )
        return FUDGE_OUT_OF_BYTES;

    /* If available for this type, use the registered decoder. Failing that,
       treat it as an array of bytes. */
    decoder = typedesc->decoder ? typedesc->decoder
                                : FudgeCodec_decodeFieldByteArray;

    memset ( &data, 0, sizeof ( data ) );

    if ( ( status = decoder ( bytes, width, &data ) ) != FUDGE_OK )
        return status;

    return FudgeMsg_addFieldData ( message,
                                   header.type,
                                   header.name,
                                   header.namelen,
                                   FudgeHeader_getOrdinal ( &header ),
                                   &data,
                                   FudgeCodec_getNumBytes ( typedesc, width ) );
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

        /* Get the field width */
        if ( ( status = FudgeHeader_getFieldWidth ( &width, &consumed, fieldheader, bytes, numbytes ) ) != FUDGE_OK )
            goto release_fieldheader_and_fail;
        bytes += consumed;
        numbytes -= consumed;

        /* Get the field and add it to the message */
        if ( ( status = FudgeCodec_decodeField ( message, fieldheader, width, bytes, numbytes ) ) != FUDGE_OK )
            goto release_fieldheader_and_fail;
        bytes += width;
        numbytes -= width;

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
 * Functions from fudge/codec_ex.h
 */

inline fudge_bool FudgeCodec_decodeBool ( const fudge_byte * bytes )
{
    return *bytes != 0;
}

inline fudge_byte FudgeCodec_decodeByte ( const fudge_byte * bytes )
{
    return *bytes;
}

#define FUDGECODEC_DECODE_PRIMITIVE_IMPL( type, typename, field, swapper )      \
    inline type FudgeCodec_decode##typename ( const fudge_byte * bytes )        \
    {                                                                           \
        return swapper ( *( ( type * ) bytes ) );                               \
    }

FUDGECODEC_DECODE_PRIMITIVE_IMPL( fudge_i16, I16, i16, ntohs )
FUDGECODEC_DECODE_PRIMITIVE_IMPL( fudge_i32, I32, i32, ntohl )
FUDGECODEC_DECODE_PRIMITIVE_IMPL( fudge_i64, I64, i64, ntohi64 )
FUDGECODEC_DECODE_PRIMITIVE_IMPL( fudge_f32, F32, f32, ntohf )
FUDGECODEC_DECODE_PRIMITIVE_IMPL( fudge_f64, F64, f64, ntohd )

inline FudgeStatus FudgeCodec_decodeByteArray ( const fudge_byte * data, const fudge_i32 width, fudge_byte * * bytes )
{
    if ( ! ( data && bytes ) )
        return FUDGE_NULL_POINTER;

    if ( width )
    {
        if ( ! ( *bytes = ( fudge_byte * ) malloc ( width ) ) )
            return FUDGE_OUT_OF_MEMORY;
        memcpy ( *bytes, data, width );
    }
    else
        *bytes = 0;

    return FUDGE_OK;
}

FudgeStatus FudgeCodec_decodeFieldByteArray ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data )
{
    return FudgeCodec_decodeByteArray ( bytes, width, ( fudge_byte * * ) &( data->bytes ) );
}


FudgeStatus FudgeCodec_decodeFieldIndicator ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data )
{
    return FUDGE_OK;
}

FudgeStatus FudgeCodec_decodeFieldBool ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data )
{
    data->boolean = FudgeCodec_decodeBool ( bytes );
    return FUDGE_OK;
}

FudgeStatus FudgeCodec_decodeFieldByte ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data )
{
    data->byte = FudgeCodec_decodeByte ( bytes );
    return FUDGE_OK;
}

/*****************************************************************************
 * Functions from ./codec_decode.h
 */

#define FUDGECODEC_DECODE_PRIMITIVE_FIELD_IMPL( type, typename, field )                                                     \
    FudgeStatus FudgeCodec_decodeField##typename ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data ) \
    {                                                                                                                       \
        data->field = FudgeCodec_decode##typename ( bytes );                                                                \
        return FUDGE_OK;                                                                                                    \
    }

FUDGECODEC_DECODE_PRIMITIVE_FIELD_IMPL( fudge_i16, I16, i16 )
FUDGECODEC_DECODE_PRIMITIVE_FIELD_IMPL( fudge_i32, I32, i32 )
FUDGECODEC_DECODE_PRIMITIVE_FIELD_IMPL( fudge_i64, I64, i64 )
FUDGECODEC_DECODE_PRIMITIVE_FIELD_IMPL( fudge_f32, F32, f32 )
FUDGECODEC_DECODE_PRIMITIVE_FIELD_IMPL( fudge_f64, F64, f64 )

#define FUDGECODEC_DECODE_ARRAY_FIELD_IMPL( type, typename, swapper )                                                               \
    FudgeStatus FudgeCodec_decodeField##typename##Array ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data )  \
    {                                                                                                                               \
        type * target;                                                                                                              \
        const type * source = ( type * ) bytes;                                                                                     \
                                                                                                                                    \
        if ( width )                                                                                                                \
        {                                                                                                                           \
            int index,                                                                                                              \
                numfields = width / sizeof ( type );                                                                                \
                                                                                                                                    \
            if ( ! ( target = ( type * ) malloc ( width ) ) )                                                                       \
                return FUDGE_OUT_OF_MEMORY;                                                                                         \
            for ( index = 0; index < numfields; ++index )                                                                           \
                target [ index ] = swapper ( source [ index ] );                                                                    \
        }                                                                                                                           \
        else                                                                                                                        \
            target = 0;                                                                                                             \
                                                                                                                                    \
        data->bytes = ( fudge_byte * ) target;                                                                                      \
        return FUDGE_OK;                                                                                                            \
    }

FUDGECODEC_DECODE_ARRAY_FIELD_IMPL( fudge_i16, I16, ntohs )
FUDGECODEC_DECODE_ARRAY_FIELD_IMPL( fudge_i32, I32, ntohl )
FUDGECODEC_DECODE_ARRAY_FIELD_IMPL( fudge_i64, I64, ntohi64 )
FUDGECODEC_DECODE_ARRAY_FIELD_IMPL( fudge_f32, F32, ntohf )
FUDGECODEC_DECODE_ARRAY_FIELD_IMPL( fudge_f64, F64, ntohd )

FudgeStatus FudgeCodec_decodeFieldFudgeMsg ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data )
{
    FudgeMsg submessage;
    FudgeStatus status;

    if ( ( status = FudgeMsg_create ( &submessage ) ) != FUDGE_OK )
        return status;
    if ( ( status = FudgeCodec_decodeMsgFields ( submessage, bytes, width ) ) != FUDGE_OK )
    {
        FudgeMsg_release ( submessage );
        return status;
    }
    data->message = submessage;

    return FUDGE_OK;
}

/*****************************************************************************
 * Functions from fudge/codec.h
 */

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

