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

#include <stdio.h>      /* TODO Remove this */

FudgeStatus FudgeCodec_decodeMsgFields ( FudgeMsg message, const fudge_byte * bytes, fudge_i32 numbytes );
FudgeStatus FudgeCodec_decodeField ( FudgeMsg message, FudgeFieldHeader header, fudge_i32 width, const fudge_byte * bytes, fudge_i32 numbytes );


FudgeStatus FudgeCodec_decodeMsg ( FudgeMsg * message, const fudge_byte * bytes, fudge_i32 numbytes )
{
    FudgeStatus status;
    FudgeMsgHeader header;

    if ( ! message )
        return FUDGE_NULL_POINTER;

    /* Get the message header and use it to create the message */
    if ( ( status = FudgeHeader_decodeMsgHeader ( &header, bytes, numbytes ) ) != FUDGE_OK )
        return status;
    if ( numbytes < header.numbytes )
        return FUDGE_OUT_OF_BYTES;
    if ( ( status = FudgeMsg_create ( message ) ) != FUDGE_OK )
        return status;

    /* TODO Populate message with schema version and taxonomy */

    /* Advance to the end of the header */
    bytes += sizeof ( FudgeMsgHeader );
    numbytes -= sizeof ( FudgeMsgHeader );

    /* Consume fields */
    if ( ( status = FudgeCodec_decodeMsgFields ( *message, bytes, numbytes ) ) != FUDGE_OK )
    {
        FudgeMsg_release ( *message );
        *message = 0;
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
        status = FudgeMsg_addField##typename##Array ( message, header.name, elements, numelements );                                            \
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
        case FUDGE_TYPE_INDICATOR:  return FudgeMsg_addFieldIndicator ( message, header.name );
        case FUDGE_TYPE_BOOLEAN:    return FudgeMsg_addFieldBool ( message, header.name, *bytes != 0 );
        case FUDGE_TYPE_BYTE:       return FudgeMsg_addFieldByte ( message, header.name, *( ( fudge_byte * ) bytes ) );
        case FUDGE_TYPE_SHORT:      return FudgeMsg_addFieldI16  ( message, header.name, ntohs ( *( ( fudge_i16 * ) bytes ) ) );
        case FUDGE_TYPE_INT:        return FudgeMsg_addFieldI32  ( message, header.name, ntohl ( *( ( fudge_i32 * ) bytes ) ) );
        case FUDGE_TYPE_LONG:       return FudgeMsg_addFieldI32  ( message, header.name, ntohi64 ( *( ( fudge_i64 * ) bytes ) ) );
        case FUDGE_TYPE_FLOAT:      return FudgeMsg_addFieldF32  ( message, header.name, ntohf ( *( ( fudge_f32 * ) bytes ) ) );
        case FUDGE_TYPE_DOUBLE:     return FudgeMsg_addFieldF64  ( message, header.name, ntohd ( *( ( fudge_f64 * ) bytes ) ) );

        /* Submessages should be a simple list of fields */
        case FUDGE_TYPE_FUDGE_MSG:
        {
            FudgeMsg submessage;

            if ( ( status = FudgeMsg_create ( &submessage ) ) != FUDGE_OK )
                return status;
            if ( ( status = FudgeCodec_decodeMsgFields ( submessage, bytes, width ) ) == FUDGE_OK )
                status = FudgeMsg_addFieldMsg ( message, header.name, submessage );
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
            return FudgeMsg_addFieldOpaque ( message, header.type, header.name, bytes, width );
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

