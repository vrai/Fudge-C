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
#include "fudge/message_ex.h"
#include "registry_internal.h"

#define FUDGEMSG_GETFIELDASPRIMITIVETYPE_IMPL( typestring, typetype, typeid, bucket )               \
    FudgeStatus FudgeMsg_getFieldAs##typestring ( const FudgeField * field, typetype * target )     \
    {                                                                                               \
        FudgeStatus status;                                                                         \
        FudgeFieldData data;                                                                        \
        fudge_i32 numbytes;                                                                         \
        const FudgeTypeDesc * typedesc = field ? FudgeRegistry_getTypeDesc ( field->type ) : 0;     \
                                                                                                    \
        if ( ! ( field && target ) )                                                                \
            return FUDGE_NULL_POINTER;                                                              \
        if ( ! typedesc )                                                                           \
            return FUDGE_INVALID_TYPE_COERCION;                                                     \
                                                                                                    \
        switch ( status = typedesc->coercer ( field, typeid, &data, &numbytes ) )                   \
        {                                                                                           \
            case FUDGE_OK:                    *target = data. bucket; return FUDGE_OK;              \
            case FUDGE_COERCION_NOT_REQUIRED: *target = field->data. bucket; return FUDGE_OK;       \
            default: return status;                                                                 \
        }                                                                                           \
    }

FUDGEMSG_GETFIELDASPRIMITIVETYPE_IMPL( Boolean, fudge_bool, FUDGE_TYPE_BOOLEAN, boolean )
FUDGEMSG_GETFIELDASPRIMITIVETYPE_IMPL( Byte,    fudge_byte, FUDGE_TYPE_BYTE,    byte )
FUDGEMSG_GETFIELDASPRIMITIVETYPE_IMPL( I16,     fudge_i16,  FUDGE_TYPE_SHORT,   i16 )
FUDGEMSG_GETFIELDASPRIMITIVETYPE_IMPL( I32,     fudge_i32,  FUDGE_TYPE_INT,     i32 )
FUDGEMSG_GETFIELDASPRIMITIVETYPE_IMPL( I64,     fudge_i64,  FUDGE_TYPE_LONG,    i64 )
FUDGEMSG_GETFIELDASPRIMITIVETYPE_IMPL( F32,     fudge_f32,  FUDGE_TYPE_FLOAT,   f32 )
FUDGEMSG_GETFIELDASPRIMITIVETYPE_IMPL( F64,     fudge_f64,  FUDGE_TYPE_DOUBLE,  f64 )

FudgeStatus FudgeMsg_getFieldAs ( const FudgeField * field,
                                  const fudge_type_id type,
                                  FudgeFieldData * target,
                                  FudgeTypePayload * payload,
                                  fudge_i32 * numbytes )
{
    const FudgeTypeDesc * fieldtypedesc,
                        * targettypedesc;

    if ( ! ( field && target && payload && numbytes ) )
        return FUDGE_NULL_POINTER;
    if ( field->type == type )
        return FUDGE_COERCION_NOT_REQUIRED;

    fieldtypedesc = FudgeRegistry_getTypeDesc ( field->type );
    targettypedesc = FudgeRegistry_getTypeDesc ( type );

    if ( fieldtypedesc && fieldtypedesc->coercer && targettypedesc )
    {
        FudgeStatus status;
        
        if ( ( status = fieldtypedesc->coercer ( field, type, target, numbytes ) ) == FUDGE_OK )
            *payload = targettypedesc->payload;

        return status;
    }
    else
        return FUDGE_INVALID_TYPE_COERCION;
}

