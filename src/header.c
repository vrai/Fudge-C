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
#include "header.h"
#include "prefix.h"
#include "fudge/platform.h"

FudgeStatus FudgeHeader_decodeMsgHeader ( FudgeMsgHeader * header, const fudge_byte * bytes, fudge_i32 numbytes )
{
    if ( numbytes < sizeof ( FudgeMsgHeader ) )
        return FUDGE_OUT_OF_BYTES;

    memcpy ( header, bytes, sizeof ( FudgeMsgHeader ) );
    header->directives = header->directives;
    header->schemaversion = header->schemaversion;
    header->taxonomy = ntohs ( header->taxonomy );
    header->numbytes = ntohl ( header->numbytes );
    return FUDGE_OK;
}

FudgeStatus FudgeHeader_decodeFieldHeader ( FudgeFieldHeader * header, fudge_i32 * consumed, const fudge_byte * bytes, fudge_i32 numbytes )
{
    FudgeStatus status;
    FudgeFieldPrefix prefix;
    fudge_i32 index = 0;

    /* The prefix is needed to know how to decode the rest of the header */
    if ( numbytes < 1 )
        return FUDGE_OUT_OF_BYTES;
    if ( ( status = FudgePrefix_decodeFieldPrefix ( &prefix, bytes [ index++ ] ) ) != FUDGE_OK )
        return status;

    /* Set message width */
    header->widthofwidth = prefix.fixedwidth ? 0 : prefix.variablewidth;

    /* The type is mandatory */
    if ( index > numbytes )
        return FUDGE_OUT_OF_BYTES;
    header->type = bytes [ index++ ];

    /* The ordinal is optional */
    if ( prefix.ordinal )
    {
        if ( index + 1 > numbytes )
            return FUDGE_OUT_OF_BYTES;
        header->hasordinal = 1;
        header->ordinal = ntohs ( *( ( fudge_i16 * ) ( bytes + index ) ) );
        index += 2;
    }
    else
    {
        header->hasordinal = 0;
        header->ordinal = 0;
    }

    /* The name is optional */
    if ( prefix.name )
    {
        fudge_byte length;

        if ( index > numbytes )
            return FUDGE_OUT_OF_BYTES;
        length = bytes [ index++ ];

        if ( index + length > numbytes )
            return FUDGE_OUT_OF_BYTES;
        if ( ! ( header->name = ( const char * ) malloc ( length + 1 ) ) )
            return FUDGE_OUT_OF_MEMORY;
        memcpy ( ( char * ) header->name, bytes + index, length );
        index += length;
        ( ( char * ) header->name ) [ length ] = 0;
    }
    else
        header->name = 0;

    *consumed = index;
    return FUDGE_OK;
}

FudgeStatus FudgeHeader_destroyFieldHeader ( FudgeFieldHeader header )
{
    free ( ( char * ) header.name );
    return FUDGE_OK;
}

FudgeStatus FudgeHeader_getFieldWidth ( fudge_i32 * width, fudge_i32 * consumed, FudgeFieldHeader header, const fudge_byte * bytes, fudge_i32 numbytes )
{
    if ( header.widthofwidth )
    {
        if ( header.widthofwidth > numbytes )
            return FUDGE_OUT_OF_BYTES;
        switch ( header.widthofwidth )
        {
            /* 1 byte widths are stored unsigned, everything else is a signed value */
            case 1: *width = ( unsigned char ) *bytes;              break;
            case 2: *width = ntohs ( *( ( fudge_i16 * ) bytes ) );  break;
            case 4: *width = ntohl ( *( ( fudge_i32 * ) bytes ) );  break;
            default:
                return FUDGE_UNKNOWN_FIELD_WIDTH;
        }
        *consumed = header.widthofwidth;
    }
    else
    {
        if ( ( *width = FudgeType_getFixedWidth ( header.type ) ) < 0 )
            return FUDGE_UNKNOWN_FIELD_WIDTH;
        *consumed = 0;
    }
    return FUDGE_OK;
}

const fudge_i16 * FudgeHeader_getOrdinal ( FudgeFieldHeader * header )
{
    return header && header->hasordinal ? &( header->ordinal ) : 0;
}

