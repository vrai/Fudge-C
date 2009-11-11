#include "fudge/message.h"
#include "fudge/reference.h"
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
                                   int type,
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
    node->field.data.message = 0;
    node->field.data.bytes = 0;
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

