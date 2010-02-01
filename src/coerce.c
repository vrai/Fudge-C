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
#include "coerce.h"

#define BYTE_BUFFER_FORMAT "%d"
#define BYTE_BUFFER_LENGTH 8

#define FUDGETYPE_COERCE_CHECK_SAME_TYPE    \
    if ( source->type == type )             \
        return FUDGE_COERCION_NOT_REQUIRED

inline const char * FudgeType_safeStringCast ( const fudge_byte * bytes )
{
    return bytes ? ( const char * ) bytes : "";
}

FudgeStatus FudgeType_coerceDefault ( const FudgeField * source, const fudge_type_id type, FudgeFieldData * target, fudge_i32 * numbytes )
{
    FUDGETYPE_COERCE_CHECK_SAME_TYPE;
    return FUDGE_INVALID_TYPE_COERCION;
}

FudgeStatus FudgeType_coerceBoolean ( const FudgeField * source, const fudge_type_id type, FudgeFieldData * target, fudge_i32 * numbytes )
{
    FUDGETYPE_COERCE_CHECK_SAME_TYPE;
    switch ( type )
    {
        case FUDGE_TYPE_BYTE:       target->byte = source->data.boolean ? 1 : 0; break;
        case FUDGE_TYPE_SHORT:      target->i16 = source->data.boolean ? 1 : 0; break;
        case FUDGE_TYPE_INT:        target->i32 = source->data.boolean ? 1 : 0; break;
        case FUDGE_TYPE_LONG:       target->i64 = source->data.boolean ? 1 : 0; break;
        default:
            return FUDGE_INVALID_TYPE_COERCION;
    }
    *numbytes = 0;
    return FUDGE_OK;
}

FudgeStatus FudgeType_coerceByte ( const FudgeField * source, const fudge_type_id type, FudgeFieldData * target, fudge_i32 * numbytes )
{
    FUDGETYPE_COERCE_CHECK_SAME_TYPE;
    switch ( type )
    {
        case FUDGE_TYPE_BOOLEAN:    target->boolean = source->data.byte != 0; break;
        case FUDGE_TYPE_SHORT:      target->i16 = ( fudge_i16 ) source->data.byte; break;
        case FUDGE_TYPE_INT:        target->i32 = ( fudge_i32 ) source->data.byte; break;
        case FUDGE_TYPE_LONG:       target->i64 = ( fudge_i64 ) source->data.byte; break;
        default:
            return FUDGE_INVALID_TYPE_COERCION;
    }
    *numbytes = 0;
    return FUDGE_OK;
}

FudgeStatus FudgeType_coerceShort ( const FudgeField * source, const fudge_type_id type, FudgeFieldData * target, fudge_i32 * numbytes )
{
    FUDGETYPE_COERCE_CHECK_SAME_TYPE;
    switch ( type )
    {
        case FUDGE_TYPE_BOOLEAN:    target->boolean = source->data.i16 != 0; break;
        case FUDGE_TYPE_INT:        target->i32 = ( fudge_i32 ) source->data.i16; break;
        case FUDGE_TYPE_LONG:       target->i64 = ( fudge_i64 ) source->data.i16; break;
        default:
            return FUDGE_INVALID_TYPE_COERCION;
    }
    *numbytes = 0;
    return FUDGE_OK;
}

FudgeStatus FudgeType_coerceInt ( const FudgeField * source, const fudge_type_id type, FudgeFieldData * target, fudge_i32 * numbytes )
{
    FUDGETYPE_COERCE_CHECK_SAME_TYPE;
    switch ( type )
    {
        case FUDGE_TYPE_BOOLEAN:    target->boolean = source->data.i32 != 0; break;
        case FUDGE_TYPE_LONG:       target->i64 = ( fudge_i64 ) source->data.i32; break;
        default:
            return FUDGE_INVALID_TYPE_COERCION;
    }
    *numbytes = 0;
    return FUDGE_OK;
}

FudgeStatus FudgeType_coerceLong ( const FudgeField * source, const fudge_type_id type, FudgeFieldData * target, fudge_i32 * numbytes )
{
    FUDGETYPE_COERCE_CHECK_SAME_TYPE;
    if ( type == FUDGE_TYPE_BOOLEAN )
    {
        target->boolean = source->data.i64 != 0;
        *numbytes = 0;
        return FUDGE_OK;
    }
    else    
        return FUDGE_INVALID_TYPE_COERCION;
}

FudgeStatus FudgeType_coerceFloat ( const FudgeField * source, const fudge_type_id type, FudgeFieldData * target, fudge_i32 * numbytes )
{
    FUDGETYPE_COERCE_CHECK_SAME_TYPE;
    if ( type == FUDGE_TYPE_DOUBLE )
    {
        target->f64 = ( fudge_f64 ) source->data.f32;
        *numbytes = 0;
        return FUDGE_OK;
    }
    else
        return FUDGE_INVALID_TYPE_COERCION;
}

FudgeStatus FudgeType_coerceDouble ( const FudgeField * source, const fudge_type_id type, FudgeFieldData * target, fudge_i32 * numbytes )
{
    FUDGETYPE_COERCE_CHECK_SAME_TYPE;
    if ( type == FUDGE_TYPE_FLOAT )
    {
        target->f32 = ( fudge_f32 ) source->data.f64;
        *numbytes = 0;
        return FUDGE_OK;
    }
    else
        return FUDGE_INVALID_TYPE_COERCION;
}

