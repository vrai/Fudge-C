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
#include "fudge/message.h"
#include "reference.h"
#include "fudge/platform.h"
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

        /* Primitive type: nothing needs freeing */
        case FUDGE_TYPE_INDICATOR:
        case FUDGE_TYPE_BOOLEAN:
        case FUDGE_TYPE_BYTE:
        case FUDGE_TYPE_SHORT:
        case FUDGE_TYPE_INT:
        case FUDGE_TYPE_LONG:
        case FUDGE_TYPE_FLOAT:
        case FUDGE_TYPE_DOUBLE:
            break;

        /* Every other type will store its data in the bytes array */
        default:
            free ( ( fudge_byte * ) node->field.data.bytes );
            break;
    }

    free ( ( char * ) node->field.name );
    free ( node );
}


struct FudgeMsgImpl
{
    FudgeRefCount refcount;
    FieldListNode * fieldhead,
                  * fieldtail;
    unsigned long numfields;
};

FudgeStatus FudgeMsg_appendField ( FudgeField * * field,
                                   FudgeMsg message,
                                   fudge_type_id type,
                                   fudge_i32 numbytes,
                                   const char * name )
{
    FieldListNode * node ;

    assert ( message );

    /* Allocate and initialise the new node, taking a copy of the name if required */
    if ( ! ( node = ( FieldListNode * ) malloc ( sizeof ( FieldListNode ) ) ) )
        return FUDGE_OUT_OF_MEMORY;
    node->next = 0;
    node->field.type = type;
    node->field.numbytes = numbytes;
    memset ( &( node->field.data ), 0, sizeof ( FudgeFieldData ) );
    if ( name )
    {
        if ( ! ( node->field.name = strdup ( name ) ) )
        {
            free ( node );
            return FUDGE_OUT_OF_MEMORY;
        }
    }
    else
        node->field.name = 0;

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
    *field = &( node->field );
    return FUDGE_OK;
}


FudgeStatus FudgeMsg_create ( FudgeMsg * messageptr )
{
    FudgeStatus status;

    if ( ! ( *messageptr = ( FudgeMsg ) malloc ( sizeof ( struct FudgeMsgImpl ) ) ) )
        return FUDGE_OUT_OF_MEMORY;

    if ( ( status = FudgeRefCount_create ( &( ( *messageptr )->refcount ) ) ) != FUDGE_OK )
        return status;
    ( *messageptr )->fieldhead = 0;
    ( *messageptr )->fieldtail = 0;
    ( *messageptr )->numfields = 0ul;

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
        /* Last reference has been released - destroyed the message and all of its fields */
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

        free ( message );
    }
    return FUDGE_OK;
}

unsigned long FudgeMsg_numFields ( FudgeMsg message )
{
    return message ? message->numfields : 0lu;
}

FudgeStatus FudgeMsg_addFieldIndicator ( FudgeMsg message, const char * name )
{
    FudgeField * field;

    if ( ! message )
        return FUDGE_NULL_POINTER;
    return FudgeMsg_appendField ( &field, message, FUDGE_TYPE_INDICATOR, 0, name );
}

#define FUDGE_ADDPRIMITIVEFIELD_IMPL( typename, type, typeid, bucket )                              \
    FudgeStatus FudgeMsg_addField##typename ( FudgeMsg message, const char * name, type value )     \
    {                                                                                               \
        FudgeStatus status;                                                                         \
        FudgeField * field;                                                                         \
                                                                                                    \
        if ( ! message )                                                                            \
            return FUDGE_NULL_POINTER;                                                              \
        if ( ( status = FudgeMsg_appendField ( &field, message, typeid, 0, name ) ) != FUDGE_OK )   \
            return status;                                                                          \
        field->data . bucket = value;                                                               \
        return FUDGE_OK;                                                                            \
    }

FUDGE_ADDPRIMITIVEFIELD_IMPL( Bool, fudge_bool, FUDGE_TYPE_BOOLEAN, boolean )
FUDGE_ADDPRIMITIVEFIELD_IMPL( Byte, fudge_byte, FUDGE_TYPE_BYTE,    byte )
FUDGE_ADDPRIMITIVEFIELD_IMPL( I16,  fudge_i16,  FUDGE_TYPE_SHORT,   i16 )
FUDGE_ADDPRIMITIVEFIELD_IMPL( I32,  fudge_i32,  FUDGE_TYPE_INT,     i32 )
FUDGE_ADDPRIMITIVEFIELD_IMPL( I64,  fudge_i64,  FUDGE_TYPE_LONG,    i64 )
FUDGE_ADDPRIMITIVEFIELD_IMPL( F32,  fudge_f32,  FUDGE_TYPE_FLOAT,   f32 )
FUDGE_ADDPRIMITIVEFIELD_IMPL( F64,  fudge_f64,  FUDGE_TYPE_DOUBLE,  f64 )

FudgeStatus FudgeMsg_addFieldMsg ( FudgeMsg message, const char * name, FudgeMsg value )
{
    FudgeStatus status;
    FudgeField * field;

    if ( ! ( message && value ) )
        return FUDGE_NULL_POINTER;
    if ( ( status = FudgeMsg_retain ( value ) ) != FUDGE_OK )
        return status;
    if ( ( status = FudgeMsg_appendField ( &field, message, FUDGE_TYPE_FUDGE_MSG, 0, name ) ) != FUDGE_OK )
    {
        FudgeMsg_release ( value );
        return status;
    }
    field->data.message = value;
    return FUDGE_OK;
}

FudgeStatus FudgeMsg_addFieldOpaque ( FudgeMsg message,
                                      fudge_type_id type,
                                      const char * name,
                                      const fudge_byte * bytes,
                                      fudge_i32 numbytes )
{
    FudgeStatus status;
    FudgeField * field;
    fudge_byte * bytebucket = 0;

    if ( ( ! message ) || ( ! bytes && numbytes ) )
        return FUDGE_NULL_POINTER;
    if ( numbytes )
        if ( ! ( bytebucket = ( fudge_byte * ) malloc ( numbytes ) ) )
            return FUDGE_OUT_OF_MEMORY;
    if ( ( status = FudgeMsg_appendField ( &field, message, type, numbytes, name ) ) != FUDGE_OK )
    {
        free ( bytebucket );
        return status;
    }
    if ( numbytes )
    {
        assert ( bytebucket );
        memcpy ( bytebucket, bytes, numbytes );
        field->data.bytes = bytebucket;
    }
    return FUDGE_OK;
}

#define FUDGE_ADDARRAYFIELD_IMPL( typename, type, typeid )                                                                               \
    FudgeStatus FudgeMsg_addField##typename##Array ( FudgeMsg message, const char * name, const type * elements, fudge_i32 numelements ) \
    {                                                                                                                                    \
        return FudgeMsg_addFieldOpaque ( message, typeid, name, ( const fudge_byte * ) elements, numelements * sizeof ( type ) );        \
    }

FUDGE_ADDARRAYFIELD_IMPL( Byte, fudge_byte, FUDGE_TYPE_BYTE_ARRAY )
FUDGE_ADDARRAYFIELD_IMPL( I16,  fudge_i16,  FUDGE_TYPE_SHORT_ARRAY )
FUDGE_ADDARRAYFIELD_IMPL( I32,  fudge_i32,  FUDGE_TYPE_INT_ARRAY )
FUDGE_ADDARRAYFIELD_IMPL( I64,  fudge_i64,  FUDGE_TYPE_LONG_ARRAY )
FUDGE_ADDARRAYFIELD_IMPL( F32,  fudge_f32,  FUDGE_TYPE_FLOAT_ARRAY )
FUDGE_ADDARRAYFIELD_IMPL( F64,  fudge_f64,  FUDGE_TYPE_DOUBLE_ARRAY )

FudgeStatus FudgeMsg_addFieldString ( FudgeMsg message, const char * name, const char * string, fudge_i32 numbytes )
{
    return FudgeMsg_addFieldOpaque ( message, FUDGE_TYPE_STRING, name, string, numbytes );
}

#define FUDGE_ADDFIXEDBYTEARRAYFIELD_IMPL( size, typeid )                                                               \
    FudgeStatus FudgeMsg_addField##size##ByteArray ( FudgeMsg message, const char * name, const fudge_byte * bytes )    \
    {                                                                                                                   \
        return FudgeMsg_addFieldOpaque ( message, typeid, name, bytes, size );                                           \
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

FudgeStatus FudgeMsg_getFieldAtIndex ( FudgeField * field, FudgeMsg message, unsigned long index )
{
    FieldListNode * node;

    if ( ! ( message &&  field ) )
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

