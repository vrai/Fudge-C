/**
 * Copyright (C) 2010 - 2010, Vrai Stacey.
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
#include <fudge/fudge.h>
#include <fudge/message.h>
#include <fudge/codec.h>
#include <fudge/string.h>
#include <stdio.h>
#include <string.h>

int logFudgeError ( FudgeStatus status, const char * context );
int logError ( const char * error );

/* This is a simple example of using the Fudge-C API to create a message,
   encoding the message, decoding the message and retrieving the field data.

   The message that is created and processed could be represented by the C
   structure:

       struct
       {
           const char * name;
           int64_t dob;
           const char * address [ ];
       };

   In this example the array of strings represented by a submessage containing
   string fields, indexed by ordinal. Note that this use of ordinals is not
   necessary as Fudge maintains field order. They are used in manner to allow
   their use to be shown while keeping the example as brief as possible.

   For the same reason the example only uses ASCII strings, albiet contained
   within the proper UTF8 container. The FudgeString container can be
   initialised with UTF8, UTF16 and UTF32 strings as well as ASCII.
*/
int main ( int argc, char * argv [ ] )
{
    /* Most Fudge-C API functions return a FudgeStatus value that indicates
       whether the call was successful and if not, what went wrong. */
    FudgeStatus status;

    /* The message containers and the temporary wrapper used when en/decoding
       the outer (contact) message */
    FudgeMsg contactMsg, addressMsg;
    FudgeMsgEnvelope envelope;

    /* Used to hold the message in its encoded form. */
    fudge_byte * bytes;
    fudge_i32 numbytes;

    /* Used to when building the example message */
    fudge_i16 index;
    const char * * stringListIterator;
    FudgeString string;

    /* Used to hold the fields when examining/output the message contents. */
    FudgeField field;
    fudge_i64 i64value;
    char * ascii;

    /* Source data for the example message */
    static const char * addressLines [ ] = { "123 Fake Street",
                                             "Some City",
                                             "P0S T4L",
                                             "Country",
                                             0 };
    static const char * name = "Random Person";

    /* Field names */
    FudgeString nameFieldName, dobFieldName, addressFieldName;
    if ( ( status = FudgeString_createFromASCIIZ ( &nameFieldName   , "name" ) ) ||
         ( status = FudgeString_createFromASCIIZ ( &dobFieldName,     "dob" ) ) ||
         ( status = FudgeString_createFromASCIIZ ( &addressFieldName, "address" ) ) )
        return logFudgeError ( status, "Failed to create field name strings" );
    
    /* The Fudge library must be initialised before it can be used */
    if ( ( status = Fudge_init ( ) ) )
        return logFudgeError ( status, "Failed to initialise library" );

    /*************************************************************************
     * Construct a Fudge Message
     */

    /* First, construct and populate the inner (address) message. This will
       hold line for a street address, with the ordinal providing ordering
       information. */
    if ( ( status = FudgeMsg_create ( &addressMsg ) ) )
        return logFudgeError ( status, "Failed to create address message" );

    for ( stringListIterator = addressLines, index = 0;
          *stringListIterator;
          ++stringListIterator, ++index )
    {
        FudgeString_createFromASCIIZ ( &string, *stringListIterator );
        FudgeMsg_addFieldString ( addressMsg,
                                  0,            /* No field name */
                                  &index,       /* Ordinal is the index */
                                  string );
        FudgeString_release ( string );
    }

    /* Now construct the outer message */
    if ( ( status = FudgeMsg_create ( &contactMsg ) ) )
        return logFudgeError ( status, "Failed to create contact message" );

    /* Add the details fields */
    FudgeString_createFromASCIIZ ( &string, name );
    FudgeMsg_addFieldString ( contactMsg,
                              nameFieldName,
                              0,
                              string );
    FudgeString_release ( string );

    FudgeMsg_addFieldI64 ( contactMsg,
                           dobFieldName,
                           0,
                           19801231 );

    /* Add the inner (address) message, once that has been done the local
       reference to the inner message should be released; the outer message
       is now holding a reference of its own. */
    if ( ( status = FudgeMsg_addFieldMsg ( contactMsg,
                                           addressFieldName,
                                           0,
                                           addressMsg ) ) )
        return logFudgeError ( status, "Failed to add address message as field" );
    FudgeMsg_release ( addressMsg );

    /*************************************************************************
     * Encode a Fudge Message
     */

    /* Before being encoded a fudge message must be wrapped in an envelope. 
       This will not grab a reference to the message it holds and is only
       intented as a temporary stack object. */
    envelope.directives = 0;
    envelope.schemaversion = 1;
    envelope.taxonomy = 0;
    envelope.message = contactMsg;

    if ( ( status = FudgeCodec_encodeMsg ( envelope, &bytes, &numbytes ) ) )
        return logFudgeError ( status, "Failed to encode contact message" );
    printf ( "Contacts message encoded as a %d byte Fudge message\n", numbytes );

    /* Now that the message has been encoded, the contacts message can be
       released. This will destroy the message and its fields (including the
       address message as the contacts message holds the only reference to
       it). */
    FudgeMsg_release ( contactMsg );

    /* Clear some of the working variables. Hopefully this should make it more
       obvious which ones the decoding process will change. */
    addressMsg = contactMsg = 0;
    memset ( &envelope, 0, sizeof ( envelope ) );

    /*************************************************************************
     * Decode a Fudge Message
     */

    if ( ( status = FudgeCodec_decodeMsg ( &envelope, bytes, numbytes ) ) )
        return logFudgeError ( status, "Failed to decode contact message" );
    printf ( "Decoded Fudge message with schema version %d and %lu fields\n",
             envelope.schemaversion,
             FudgeMsg_numFields ( envelope.message ) );

    /* Now the message has been decoded it's safe to free the byte array
       holding the encoded message. */
    free ( bytes );

    /* Re-use the local contacts message variable to save on typing. Don't
       increase the reference count as the calling code is already considered
       to hold a reference after decoding (which must be released by the
       calling code when the message is no longer needed). */
    contactMsg = envelope.message;

    /* Retrieve and output the name; having checked that it is a string. There
       is no need to clear the field after use or free any memory as it is
       merely a reference to memory held by its parent message. */
    if ( ( status = FudgeMsg_getFieldByName ( &field, contactMsg, nameFieldName ) ) )
        return logFudgeError ( status, "Failed to retrieve field \"name\"" );
    if ( field.type != FUDGE_TYPE_STRING )
        return logError ( "Field \"name\" is not a string" );
    FudgeString_convertToASCIIZ ( &ascii, field.data.string );
    printf ( "Name (%d bytes): \"%s\"\n", field.numbytes, ascii );
    free ( ascii );

    /* Retrieve and output the date-of-birth field. While this was added as
       a 64bit integer it should have been stored as a 32bit value (the
       smallest integer type that can hold the value provided). */
    if ( ( status = FudgeMsg_getFieldByName ( &field, contactMsg, dobFieldName ) ) )
        return logFudgeError ( status, "Failed to retrieve field \"dob\"" );
    if ( field.type != FUDGE_TYPE_INT )
        return logError ( "Field \"dob\" is not a 32bit integer" );
    if ( ( status = FudgeMsg_getFieldAsI64 ( &field, &i64value ) ) )
        return logFudgeError ( status, "Failed to cast from I32 to I64" );
    printf ( "DOB: %lu\n", ( unsigned long ) i64value );

    /* Retrieve the inner (address) message and place it in the local address
       message variable. Grab a reference to the message as it needs to persit
       beyond the lifetime of its parent message. */
    if ( ( status = FudgeMsg_getFieldByName ( &field, contactMsg, addressFieldName ) ) )
        return logFudgeError ( status, "Failed to retrieve field \"address\"" );
    addressMsg = field.data.message;
    FudgeMsg_retain ( addressMsg );

    /* Release the outer (contacts) message. This should free the memory it
       was using, with the exception of the address message as an additional
       reference has been retained for it. */
    FudgeMsg_release ( contactMsg );

    /* Referencing the fields by ordinal, retrieve the address lines. */
    for ( index = 0; index < FudgeMsg_numFields ( addressMsg ); ++index )
    {
        if ( ( status = FudgeMsg_getFieldByOrdinal ( &field, addressMsg, index ) ) )
            return logFudgeError ( status, "Failed to retrieve address field" );
        if ( field.type != FUDGE_TYPE_STRING )
            return logError ( "Address field is not a string" );
        FudgeString_convertToASCIIZ ( &ascii, field.data.string );
        printf ( "Address line %d: \"%s\" (%d bytes)\n", index, ascii, field.numbytes );
        free ( ascii );
    }

    /* Release the address message and field name strings, this should leave
       no memory left allocated that isn't static. */
    FudgeMsg_release ( addressMsg );
    FudgeString_release ( nameFieldName );
    FudgeString_release ( dobFieldName );
    FudgeString_release ( addressFieldName );
    return 0;
}

int logFudgeError ( FudgeStatus status, const char * context )
{
    if ( status )
        printf ( "Fudge Error: %s : %s\n", context,
                 FudgeStatus_strerror ( status ) );
    return status;
}

int logError ( const char * error )
{
    printf ( "Error: %s\n", error );
    return 1;
}

