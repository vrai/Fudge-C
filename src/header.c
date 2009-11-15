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

FudgeStatus FudgeHeader_decodeFieldPrefix ( FudgeFieldPrefix * prefix, fudge_byte byte )
{
    prefix->fixedwidth = ( byte & 0x80 ) != 0;
    prefix->ordinal = ( byte & 0x10 ) != 0;
    prefix->name = ( byte & 0x08 ) != 0;

    prefix->variablewidth = ( byte & 0x60 ) >> 5;
    if ( prefix->variablewidth == 3 )
        prefix->variablewidth = 4;

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
    if ( ( status = FudgeHeader_decodeFieldPrefix ( &prefix, bytes [ index++ ] ) ) != FUDGE_OK )
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
        header->ordinal = *( ( fudge_i16 * ) ( bytes + index ) );
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

fudge_i32 FudgeHeader_getFixedFieldWidth ( fudge_type_id type )
{
    switch ( type )
    {
        case FUDGE_TYPE_INDICATOR:  return 0;
        case FUDGE_TYPE_BOOLEAN:    return 1;
        case FUDGE_TYPE_BYTE:       return 1;
        case FUDGE_TYPE_SHORT:      return 2;
        case FUDGE_TYPE_INT:        return 4;
        case FUDGE_TYPE_LONG:       return 8;
        case FUDGE_TYPE_FLOAT:      return 4;
        case FUDGE_TYPE_DOUBLE:     return 8;

        case FUDGE_TYPE_BYTE_ARRAY_4:   return 4;
        case FUDGE_TYPE_BYTE_ARRAY_8:   return 8;
        case FUDGE_TYPE_BYTE_ARRAY_16:  return 16;
        case FUDGE_TYPE_BYTE_ARRAY_20:  return 20;
        case FUDGE_TYPE_BYTE_ARRAY_32:  return 32;
        case FUDGE_TYPE_BYTE_ARRAY_64:  return 64;
        case FUDGE_TYPE_BYTE_ARRAY_128: return 128;
        case FUDGE_TYPE_BYTE_ARRAY_256: return 256;
        case FUDGE_TYPE_BYTE_ARRAY_512: return 512;

        default:
            return -1;
    }
}

FudgeStatus FudgeHeader_getFieldWidth ( fudge_i32 * width, fudge_i32 * consumed, FudgeFieldHeader header, const fudge_byte * bytes, fudge_i32 numbytes )
{
    if ( header.widthofwidth )
    {
        if ( header.widthofwidth > numbytes )
            return FUDGE_OUT_OF_BYTES;
        switch ( header.widthofwidth )
        {
            case 1: *width = *( ( fudge_byte * ) bytes );           break;
            case 2: *width = ntohs ( *( ( fudge_i16 * ) bytes ) );  break;
            case 4: *width = ntohl ( *( ( fudge_i32 * ) bytes ) );  break;
            default:
                return FUDGE_UNKNOWN_FIELD_WIDTH;
        }
        *consumed = header.widthofwidth;
    }
    else
    {
        if ( ( *width = FudgeHeader_getFixedFieldWidth ( header.type ) ) < 0 )
            return FUDGE_UNKNOWN_FIELD_WIDTH;
        *consumed = 0;
    }
    return FUDGE_OK;
}

