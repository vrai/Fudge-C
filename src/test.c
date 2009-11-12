#include "reference.h"
#include "fudge/message.h"
#include <stdio.h>
#include <math.h>

void testFloat ( float input )
{
    float output, original;

    output = htonf ( input );
    printf ( "Convert float %f to network: %f\n", input, output );
    original = ntohf ( output );
    printf ( "Convert float %f to host: %f\n", output, original );
    printf ( "Difference after feedback: %f\n\n", input - original );
}

void testDouble ( double input )
{
    double output, original;

    output = htond ( input );
    printf ( "Convert double %e to network: %e\n", input, output );
    original = ntohd ( output );
    printf ( "Convert double %e to host: %e\n", output, original );
    printf ( "Difference after feedback: %e\n\n", input - original );
}

void testI64 ( int64_t input )
{
    int64_t output, original;

    output = htoni64 ( input );
    printf ( "Convert i64 %lld to network: %lld\n", input, output );
    original = ntohi64 ( output );
    printf ( "Convert float %lld to host: %lld\n", output, original );
    printf ( "Difference after feedback: %lld\n\n", input - original );
}

void testMessages ( )
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

    printf ( "Adding indicator message: %s\n", FudgeStatus_strerror ( FudgeMsg_addFieldIndicator ( message, 0 ) ) );
    printf ( "Adding bool to message: %s\n", FudgeStatus_strerror ( FudgeMsg_addFieldBool ( message, 0, FUDGE_TRUE ) ) );
    printf ( "Fields in message: %lu\n", FudgeMsg_numFields ( message ) );

    printf ( "Releasing message: %s\n", FudgeStatus_strerror ( FudgeMsg_release ( message ) ) );
}

int main ( int argc, char * argv [ ] )
{
    testFloat ( 0.0f );
#ifdef NAN
    testFloat ( NAN );
#else
    testFloat ( ( float ) 0x7fc00000 );
#endif
    testFloat ( 1.23456f );
    testFloat ( 2E31f );

    testDouble ( 0.0 );
    testDouble ( ( double ) 0x7ff8000000000000ll );
    testDouble ( 1E-63 );
    testDouble ( 2.1E61 );

    testI64 ( 0ll );
    testI64 ( ( int64_t ) 2E31 );
    testI64 ( ( int64_t ) -2E31 );
    testI64 ( 123ll );
    testI64 ( 456ll );

    testMessages ( );

    return 0;
}

