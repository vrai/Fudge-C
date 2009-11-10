#include "reference.h"
#include "message.h"
#include <stdio.h>

int main ( int argc, char * argv [ ] )
{
    int index;
    FudgeRefCount refcount;
    FudgeMsg message, submessage;

    /* Simple reference counting test */
    printf ( "Creating refcount: %s\n", FudgeStatus_strerror ( FudgeRefCount_create ( &refcount ) ) );
    printf ( "Get refcount: %u\n", FudgeRefCount_count ( refcount ) );

    FudgeRefCount_increment ( refcount );
    printf ( "Get post-increment refcount: %u\n", FudgeRefCount_count ( refcount ) );
    
    printf ( "Get post-decrement refcount: %u\n", FudgeRefCount_decrementAndReturn ( refcount ) );

    for ( index = 0; index < 5; ++index )
        FudgeRefCount_increment ( refcount );
    printf ( "Get post-increment (x5) refcount: %u\n", FudgeRefCount_count ( refcount ) );
    
    for ( index = 0; index < 6; ++index )
        printf ( "Get post-decrement refcount: %u\n", FudgeRefCount_decrementAndReturn ( refcount ) );

    printf ( "Get refcount: %u\n", FudgeRefCount_count ( refcount ) );
    printf ( "Destroying refcount: %s\n", FudgeStatus_strerror ( FudgeRefCount_destroy ( refcount ) ) );

    /* Message test */
    printf ( "Creating message: %s\n", FudgeStatus_strerror ( FudgeMsg_create ( &message ) ) );
    printf ( "Fields in message: %lu\n", FudgeMsg_numFields ( message ) );

    printf ( "Creating submessage: %s\n", FudgeStatus_strerror ( FudgeMsg_create ( &submessage ) ) );
    printf ( "Adding submessage to message: %s\n", FudgeStatus_strerror ( FudgeMsg_addFieldMsg ( message, "SubMessage", submessage ) ) );
    printf ( "Releasing submessage: %s\n", FudgeStatus_strerror ( FudgeMsg_release ( submessage ) ) );
    
    printf ( "Fields in message: %lu\n", FudgeMsg_numFields ( message ) );
    printf ( "Releasing message: %s\n", FudgeStatus_strerror ( FudgeMsg_release ( message ) ) );

    return 0;
}

