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

#define _FUDGEMSGIMPL_DEFINED 1
#include "fudge/message.h"
#include "fudge/platform.h"
#include "fudge/string.h"
#include "memory_internal.h"
#include "message_internal.h"
#include "fudge/header.h"
#include "reference.h"
#include "registry_internal.h"
#include <assert.h>

typedef struct FieldListNode
{
    FudgeField field;
    struct FieldListNode * next;
} FieldListNode;

void FieldListNode_destroy ( FieldListNode * node )
{
    assert ( node );

    switch ( node->field.type )
    {
        /* Fudge message type: free the message pointer */
        case FUDGE_TYPE_FUDGE_MSG:
            if ( node->field.data.message )
                FudgeMsg_release ( node->field.data.message );
            break;

        /* Fudge string type: free the string pointer */
        case FUDGE_TYPE_STRING:
            if ( node->field.data.string )
                FudgeString_release ( node->field.data.string );
            break;

        /* Primitive type: nothing needs freeing */
        case FUDGE_TYPE_INDICATOR:
        case FUDGE_TYPE_BOOLEAN:
        case FUDGE_TYPE_BYTE:
        case FUDGE_TYPE_SHORT:
        case FUDGE_TYPE_INT:
        case FUDGE_TYPE_LONG:
        case FUDGE_TYPE_FLOAT:
        case FUDGE_TYPE_DOUBLE:
        case FUDGE_TYPE_DATE:
        case FUDGE_TYPE_TIME:
        case FUDGE_TYPE_DATETIME:
            break;

        /* Every other type will store its data in the bytes array */
        default:
            FUDGEMEMORY_FREE( ( fudge_byte * ) node->field.data.bytes );
            break;
    }

    if ( node->field.name )
        FudgeString_release ( node->field.name );
    FUDGEMEMORY_FREE( node );
}

struct FudgeMsgImpl
{
    FudgeRefCount refcount;
    FieldListNode * fieldhead,
                  * fieldtail;
    unsigned long numfields;
    fudge_i32 width;
};

FudgeStatus FudgeMsg_addFieldData ( FudgeMsg message,
                                    fudge_type_id type,
                                    const FudgeString name,
                                    const fudge_i16 * ordinal,
                                    FudgeFieldData * data,
                                    fudge_i32 numbytes )
{
    FudgeStatus status;
    FieldListNode * node;
    const FudgeTypeDesc * typedesc = FudgeRegistry_getTypeDesc ( type );

    if ( ! ( message && data ) )
        return FUDGE_NULL_POINTER;

    /* Adding a field will invalidate the message's width */
    message->width = -1;

    /* Allocate and initialise the new node, taking a copy of the name if required */
    if ( ! ( node = FUDGEMEMORY_MALLOC( FieldListNode *, sizeof ( FieldListNode ) ) ) )
        return FUDGE_OUT_OF_MEMORY;
    node->next = 0;
    node->field.type = type;
    node->field.numbytes = numbytes;
    node->field.flags = 0;

    /* Copy across the data */
    if ( typedesc->payload == FUDGE_TYPE_PAYLOAD_SUBMSG && ! data->message )
        return FUDGE_NULL_POINTER;
    else if ( typedesc->payload == FUDGE_TYPE_PAYLOAD_STRING && ! data->string )
        return FUDGE_NULL_POINTER;
    node->field.data = *data;

    /* Set the field name (if required) */
    if ( name )
    {
        /* Names may not have a length greater than 255 bytes (only one byte is
           available for their length) */
        if ( FudgeString_getSize ( name ) >= 256 )
        {
            status = FUDGE_NAME_TOO_LONG;
            goto release_node_and_fail;
        }

        if ( ( status = FudgeString_retain ( name ) ) != FUDGE_OK )
            goto release_node_and_fail;

        node->field.name = name;
        node->field.flags |= FUDGE_FIELD_HAS_NAME;
    }
    else
    {
        node->field.name = 0;
    }

    /* Set the field ordinal (if required) */
    if ( ordinal )
    {
        node->field.ordinal = *ordinal;
        node->field.flags |= FUDGE_FIELD_HAS_ORDINAL;
    }
    else
        node->field.ordinal = 0;

    /* Append the node to the message's list */
    if ( message->fieldtail )
    {
        if ( message->fieldtail->next )
        {
            /* Field tail is pointing at a non-tail node - something's gone very wrong */
            assert ( message->fieldtail->next == 0 );
            FieldListNode_destroy ( node );
            return FUDGE_INTERNAL_LIST_STATE;
        }

        message->fieldtail->next = node;
        message->fieldtail = node;
    }
    else
        message->fieldhead = message->fieldtail = node;

    message->numfields += 1ul;
    return FUDGE_OK;

release_node_and_fail:
    FUDGEMEMORY_FREE( node );
    return status;
}

FudgeStatus FudgeMsg_create ( FudgeMsg * messageptr )
{
    FudgeStatus status;

    if ( ! ( *messageptr = FUDGEMEMORY_MALLOC( FudgeMsg, sizeof ( struct FudgeMsgImpl ) ) ) )
        return FUDGE_OUT_OF_MEMORY;

    if ( ( status = FudgeRefCount_create ( &( ( *messageptr )->refcount ) ) ) != FUDGE_OK )
    {
        FUDGEMEMORY_FREE( *messageptr );
        return status;
    }

    ( *messageptr )->fieldhead = 0;
    ( *messageptr )->fieldtail = 0;
    ( *messageptr )->numfields = 0ul;
    ( *messageptr )->width = -1;

    return FUDGE_OK;
}

FudgeStatus FudgeMsg_retain ( FudgeMsg message )
{
    if ( ! message )
        return FUDGE_NULL_POINTER;

    FudgeRefCount_increment ( message->refcount );
    return FUDGE_OK;
}

FudgeStatus FudgeMsg_release ( FudgeMsg message )
{
    if ( ! message )
        return FUDGE_NULL_POINTER;

    if ( ! FudgeRefCount_decrementAndReturn ( message->refcount ) )
    {
        /* Last reference has been released - destroy the message and all of its fields */
        FudgeStatus status;
        FieldListNode * node;

        if ( ( status = FudgeRefCount_destroy ( message->refcount ) ) != FUDGE_OK )
            return status;

        while ( message->fieldhead )
        {
            node = message->fieldhead;
            message->fieldhead = node->next;
            FieldListNode_destroy ( node );
        }

        FUDGEMEMORY_FREE( message );
    }
    return FUDGE_OK;
}

unsigned long FudgeMsg_numFields ( FudgeMsg message )
{
    return message ? message->numfields : 0lu;
}

FudgeStatus FudgeMsg_addFieldIndicator ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal )
{
    FudgeFieldData data;
    if ( ! message )
        return FUDGE_NULL_POINTER;
    return FudgeMsg_addFieldData ( message, FUDGE_TYPE_INDICATOR, name, ordinal, &data, 0 );
}

fudge_type_id FudgeMsg_pickIntegerType ( const fudge_type_id type, const fudge_i64 value )
{
    if ( type == FUDGE_TYPE_BYTE || ( value <= INT8_MAX && value >= INT8_MIN ) )
        return FUDGE_TYPE_BYTE;
    if ( type == FUDGE_TYPE_SHORT || ( value <= INT16_MAX && value >= INT16_MIN ) )
        return FUDGE_TYPE_SHORT;
    if ( type == FUDGE_TYPE_INT || ( value <= INT32_MAX && value >= INT32_MIN ) )
        return FUDGE_TYPE_INT;
    return FUDGE_TYPE_LONG;
}

FudgeStatus FudgeMsg_addFieldInteger ( FudgeMsg message, 
                                       const FudgeString name,
                                       const fudge_i16 * ordinal,
                                       fudge_type_id type,
                                       const fudge_i64 value )
{
    FudgeFieldData data;

    if ( ! message )
        return FUDGE_NULL_POINTER;

    switch ( ( type = FudgeMsg_pickIntegerType ( type, value ) ) )
    {
        case FUDGE_TYPE_LONG:   data.i64 = value; break;
        case FUDGE_TYPE_INT:    data.i32 = ( fudge_i32 ) value; break;
        case FUDGE_TYPE_SHORT:  data.i16 = ( fudge_i16 ) value; break;
        case FUDGE_TYPE_BYTE:   data.byte = ( fudge_byte ) value; break;
        default:
            return FUDGE_INTERNAL_PAYLOAD;
    }
    
    return FudgeMsg_addFieldData ( message, type, name, ordinal, &data, 0 );
}

#define FUDGE_ADDINTEGERFIELD_IMPL( typename, type, typeid )                                                                    \
    FudgeStatus FudgeMsg_addField##typename ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, type value ) \
    {                                                                                                                           \
        return FudgeMsg_addFieldInteger ( message, name, ordinal, typeid, ( fudge_i64 ) value );                                \
    }

FUDGE_ADDINTEGERFIELD_IMPL( I16,  fudge_i16,  FUDGE_TYPE_SHORT )
FUDGE_ADDINTEGERFIELD_IMPL( I32,  fudge_i32,  FUDGE_TYPE_INT )
FUDGE_ADDINTEGERFIELD_IMPL( I64,  fudge_i64,  FUDGE_TYPE_LONG )

#define FUDGE_ADDPRIMITIVEFIELD_IMPL( typename, type, typeid, bucket )                                                                      \
    FudgeStatus FudgeMsg_addField##typename ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, type value )             \
    {                                                                                                                                       \
        FudgeFieldData data;                                                                                                                \
                                                                                                                                            \
        if ( ! message )                                                                                                                    \
            return FUDGE_NULL_POINTER;                                                                                                      \
        data . bucket = value;                                                                                                              \
        return FudgeMsg_addFieldData ( message, typeid, name, ordinal, &data, 0 );    \
    }

FUDGE_ADDPRIMITIVEFIELD_IMPL( Byte, fudge_byte, FUDGE_TYPE_BYTE,    byte )
FUDGE_ADDPRIMITIVEFIELD_IMPL( Bool, fudge_bool, FUDGE_TYPE_BOOLEAN, boolean )
FUDGE_ADDPRIMITIVEFIELD_IMPL( F32,  fudge_f32,  FUDGE_TYPE_FLOAT,   f32 )
FUDGE_ADDPRIMITIVEFIELD_IMPL( F64,  fudge_f64,  FUDGE_TYPE_DOUBLE,  f64 )

FudgeStatus FudgeMsg_addFieldMsg ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, FudgeMsg value )
{
    FudgeStatus status;
    FudgeFieldData data;

    if ( ! ( message && value ) )
        return FUDGE_NULL_POINTER;
    if ( ( status = FudgeMsg_retain ( value ) ) != FUDGE_OK )
        return status;
    data.message = value;
    if ( ( status = FudgeMsg_addFieldData ( message, FUDGE_TYPE_FUDGE_MSG, name, ordinal, &data, 0 ) ) != FUDGE_OK )
    {
        FudgeMsg_release ( value );
        return status;
    }
    return FUDGE_OK;
}

FudgeStatus FudgeMsg_addFieldOpaque ( FudgeMsg message,
                                      fudge_type_id type,
                                      const FudgeString name,
                                      const fudge_i16 * ordinal,
                                      const fudge_byte * bytes,
                                      fudge_i32 numbytes )
{
    FudgeStatus status;
    FudgeFieldData data;

    if ( ( ! message ) || ( ! bytes && numbytes ) )
        return FUDGE_NULL_POINTER;

    if ( numbytes )
    {
        if ( ! ( data.bytes = FUDGEMEMORY_MALLOC( fudge_byte *, numbytes ) ) )
            return FUDGE_OUT_OF_MEMORY;
        memcpy ( ( fudge_byte * ) data.bytes, bytes, numbytes );
    }
    else
        data.bytes = 0;

    if ( ( status = FudgeMsg_addFieldData ( message, type, name, ordinal, &data, numbytes ) ) != FUDGE_OK )
        FUDGEMEMORY_FREE( ( fudge_byte * ) data.bytes );
    return status;
}

#define FUDGE_ADDARRAYFIELD_IMPL( typename, type, typeid )                                                          \
    FudgeStatus FudgeMsg_addField##typename##Array ( FudgeMsg message,                                              \
                                                     const FudgeString name,                                        \
                                                     const fudge_i16 * ordinal,                                     \
                                                     const type * elements,                                         \
                                                     fudge_i32 numelements )                                        \
    {                                                                                                               \
        fudge_i32 maxelements = 0x7FFFFFFF / sizeof ( type );                                                       \
        if ( numelements > maxelements )                                                                            \
            return FUDGE_PAYLOAD_TOO_LONG;                                                                          \
        return FudgeMsg_addFieldOpaque ( message, typeid, name, ordinal,                                            \
                                         ( const fudge_byte * ) elements, sizeof ( type ) * numelements );          \
    }

FUDGE_ADDARRAYFIELD_IMPL( Byte, fudge_byte, FUDGE_TYPE_BYTE_ARRAY )
FUDGE_ADDARRAYFIELD_IMPL( I16,  fudge_i16,  FUDGE_TYPE_SHORT_ARRAY )
FUDGE_ADDARRAYFIELD_IMPL( I32,  fudge_i32,  FUDGE_TYPE_INT_ARRAY )
FUDGE_ADDARRAYFIELD_IMPL( I64,  fudge_i64,  FUDGE_TYPE_LONG_ARRAY )
FUDGE_ADDARRAYFIELD_IMPL( F32,  fudge_f32,  FUDGE_TYPE_FLOAT_ARRAY )
FUDGE_ADDARRAYFIELD_IMPL( F64,  fudge_f64,  FUDGE_TYPE_DOUBLE_ARRAY )

FudgeStatus FudgeMsg_addFieldString ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, FudgeString string )
{
    FudgeStatus status;
    FudgeFieldData data;
    size_t stringbytes;

    if ( ! ( message && string ) )
        return FUDGE_NULL_POINTER;
    stringbytes = FudgeString_getSize ( string );
    if ( stringbytes > 0x7FFFFFFF )
        return FUDGE_PAYLOAD_TOO_LONG;
    if ( ( status = FudgeString_retain ( string ) ) != FUDGE_OK )
        return status;
    data.string = string;

    if ( ( status = FudgeMsg_addFieldData ( message, FUDGE_TYPE_STRING, name, ordinal, &data, ( fudge_i32 ) stringbytes ) ) != FUDGE_OK )
        FudgeString_release ( string );
    return status;
}

#define FUDGE_ADDFIXEDBYTEARRAYFIELD_IMPL( size, typeid )                                                       \
    FudgeStatus FudgeMsg_addField##size##ByteArray ( FudgeMsg message,                                          \
                                                     const FudgeString name,                                    \
                                                     const fudge_i16 * ordinal,                                 \
                                                     const fudge_byte * bytes )                                 \
    {                                                                                                           \
        return FudgeMsg_addFieldOpaque ( message, typeid, name, ordinal, bytes, size );                         \
    }

FUDGE_ADDFIXEDBYTEARRAYFIELD_IMPL( 4, FUDGE_TYPE_BYTE_ARRAY_4 )
FUDGE_ADDFIXEDBYTEARRAYFIELD_IMPL( 8, FUDGE_TYPE_BYTE_ARRAY_8 )
FUDGE_ADDFIXEDBYTEARRAYFIELD_IMPL( 16, FUDGE_TYPE_BYTE_ARRAY_16 )
FUDGE_ADDFIXEDBYTEARRAYFIELD_IMPL( 20, FUDGE_TYPE_BYTE_ARRAY_20 )
FUDGE_ADDFIXEDBYTEARRAYFIELD_IMPL( 32, FUDGE_TYPE_BYTE_ARRAY_32 )
FUDGE_ADDFIXEDBYTEARRAYFIELD_IMPL( 64, FUDGE_TYPE_BYTE_ARRAY_64 )
FUDGE_ADDFIXEDBYTEARRAYFIELD_IMPL( 128, FUDGE_TYPE_BYTE_ARRAY_128 )
FUDGE_ADDFIXEDBYTEARRAYFIELD_IMPL( 256, FUDGE_TYPE_BYTE_ARRAY_256 )
FUDGE_ADDFIXEDBYTEARRAYFIELD_IMPL( 512, FUDGE_TYPE_BYTE_ARRAY_512 )

FudgeStatus FudgeMsg_addFieldDate ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, const FudgeDate * date )
{
    FudgeFieldData data;
    if ( ! ( message && date ) )
        return FUDGE_NULL_POINTER;
    data.datetime.date = *date;
    return FudgeMsg_addFieldData ( message, FUDGE_TYPE_DATE, name, ordinal, &data, 0 );
    return FUDGE_OK;
}

FudgeStatus FudgeMsg_addFieldTime ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, const FudgeTime * time )
{
    FudgeFieldData data;

    if ( ! ( message && time ) )
        return FUDGE_NULL_POINTER;
    data.datetime.time = *time;
    return FudgeMsg_addFieldData ( message, FUDGE_TYPE_TIME, name, ordinal, &data, 0 );
}

FudgeStatus FudgeMsg_addFieldDateTime ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, const FudgeDateTime * datetime )
{
    FudgeFieldData data;

    if ( ! ( message && datetime ) )
        return FUDGE_NULL_POINTER;

    data.datetime = *datetime;
    return FudgeMsg_addFieldData ( message, FUDGE_TYPE_DATETIME, name, ordinal, &data, 0 );
}

FudgeStatus FudgeMsg_getFieldAtIndex ( FudgeField * field, const FudgeMsg message, unsigned long index )
{
    FieldListNode * node;

    if ( ! ( message && field ) )
        return FUDGE_NULL_POINTER;
    if ( index >= message->numfields )
        return FUDGE_INVALID_INDEX;

    node = message->fieldhead;
    while ( node && index-- )
        node = node->next;

    if ( node )
    {
        *field = node->field;
        return FUDGE_OK;
    }
    else
        return FUDGE_INTERNAL_LIST_STATE;
}

FudgeStatus FudgeMsg_getFieldByName ( FudgeField * field, const FudgeMsg message, const FudgeString name )
{
    FieldListNode * node;

    if ( ! ( message && field && name ) )
        return FUDGE_NULL_POINTER;

    for ( node = message->fieldhead; node; node = node->next )
    {
        if ( FudgeString_compare ( node->field.name, name ) == 0 )
        {
            *field = node->field;
            return FUDGE_OK;
        }
    }

    return FUDGE_INVALID_NAME;
}

FudgeStatus FudgeMsg_getFieldByOrdinal ( FudgeField * field, const FudgeMsg message, fudge_i16 ordinal )
{
    FieldListNode * node;

    if ( ! ( message && field ) )
        return FUDGE_NULL_POINTER;

    for ( node = message->fieldhead; node; node = node->next )
        if ( node->field.flags & FUDGE_FIELD_HAS_ORDINAL && node->field.ordinal == ordinal )
        {
            *field = node->field;
            return FUDGE_OK;
        }

    return FUDGE_INVALID_ORDINAL;
}

fudge_i32 FudgeMsg_getFields ( FudgeField * fields, fudge_i32 numfields, const FudgeMsg message )
{
    FieldListNode * node;
    fudge_i32 index;

    if ( ! ( fields && message && numfields >= 0 ) )
        return -1;

    for ( index = 0, node = message->fieldhead; index < numfields && node; ++index, node = node->next )
        fields [ index ] = node->field;

    return index;
}

FudgeStatus FudgeMsg_setWidth ( FudgeMsg message, fudge_i32 width )
{
    if ( ! message )
        return FUDGE_NULL_POINTER;

    message->width = width;
    return FUDGE_OK;
}

fudge_i32 FudgeMsg_getWidth ( const FudgeMsg message )
{
    return message ? message->width : -1;
}

