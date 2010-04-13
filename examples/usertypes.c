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
#include <fudge/envelope.h>
#include <fudge/codec.h>
#include <fudge/string.h>
#include <stdio.h>

/* For more basic examples of Fudge-C use, see the "simple.c" and
   "prettyprint.c" files that should be in the same directory as this one.

   This file demostrates the registering and use of user types with the Fudge
   API.

   A custom type must have an encoding function (takes a FudgeField instance
   holding the type and encodes it on to an array of bytes), a decoding
   function (constructs a FudgeField instance of the type from an array of
   of bytes) and a type coercion function (converts a FudgeField instance of
   the type in to other types).

   In addition to these, the example uses a custom addField implementation.
   This is not required but is recommended as it allows the new type to added
   to messages in the same manner as built in types.
*/

/* The user type - a simple address details record */
typedef enum
{
    Status_Unknown,
    Status_Active,
    Status_Past
} AddressStatus;

#define ADDRESSDETAILS_NAME_FIELD_LEN 64
#define ADDRESSDETAILS_CITY_FIELD_LEN 32
#define ADDRESSDETAILS_POST_FIELD_LEN 10

typedef struct
{
    AddressStatus status;
    fudge_i16 house_number;
    char street_name [ ADDRESSDETAILS_NAME_FIELD_LEN ];
    char city [ ADDRESSDETAILS_CITY_FIELD_LEN ];
    char postal_code [ ADDRESSDETAILS_POST_FIELD_LEN ];
} AddressDetails;

#define FUDGE_TYPE_ADDRESSDETAILS 32

/* Utility field to add an AddressDetails structure to a FudgeMsg. Not
   required, but allows the user type to match the standard Fudge-C API. */
FudgeStatus FudgeMsg_addFieldAddressDetails ( FudgeMsg message,
                                              const FudgeString name, 
                                              const fudge_i16 * ordinal,
                                              const AddressDetails * details )
{
    if ( ! ( message && details ) )
        return FUDGE_NULL_POINTER;

    return FudgeMsg_addFieldOpaque ( message,
                                     FUDGE_TYPE_ADDRESSDETAILS,
                                     name,
                                     ordinal,
                                     ( const fudge_byte * ) details,
                                     sizeof ( AddressDetails ) );
}

/* Encoding and decoding functions for AddressDetails. These are required
   if a type is to be used by Fudge. */
FudgeStatus FudgeCodec_encodeFieldAddressDetails ( const FudgeField * field,
                                                   fudge_byte * * data )
{
    /* The FudgeMsg_addFieldAddressDetails would have simply copied the
       structure in to a byte array. */
    const AddressDetails * details = ( const AddressDetails * ) field->data.bytes;

    /* First, encode the length of the field */
    FudgeCodec_encodeFieldLength ( field->numbytes, data );

    /* Second, add the fields in turn. Use the encoding functions provided
       by the Fudge API to ensure endian correctness and that the target
       pointer is correctly advanced after each write. */
    FudgeCodec_encodeI32 ( ( fudge_i32 ) details->status, data );
    FudgeCodec_encodeI16 ( details->house_number, data );
    FudgeCodec_encodeByteArray ( ( const fudge_byte * ) details->street_name,
                                 ADDRESSDETAILS_NAME_FIELD_LEN,
                                 FUDGE_TRUE, data );
    FudgeCodec_encodeByteArray ( ( const fudge_byte * ) details->city,
                                 ADDRESSDETAILS_CITY_FIELD_LEN,
                                 FUDGE_TRUE, data );
    FudgeCodec_encodeByteArray ( ( const fudge_byte * ) details->postal_code,
                                 ADDRESSDETAILS_POST_FIELD_LEN,
                                 FUDGE_TRUE, data );
    return FUDGE_OK;
}

FudgeStatus FudgeCodec_decodeFieldAddressDetails ( const fudge_byte * bytes,
                                                   const fudge_i32 width,
                                                   FudgeFieldData * data )
{
    AddressDetails * details;

    /* Make sure that field is of the expected size */
    if ( width != sizeof ( AddressDetails ) )
        return FUDGE_OUT_OF_BYTES;

    /* Allocate the target structure and point the data structure's byte
       pointer at it. This memory will be owned by the parent FudgeMsg. */
    if ( ! ( details = ( AddressDetails * ) malloc ( width ) ) )
        return FUDGE_OUT_OF_MEMORY;
    data->bytes = ( const fudge_byte * ) details;

    /* Decode and copy in the integer values, advancing the source bytes
       pointer after each one */
    details->status = ( AddressStatus ) FudgeCodec_decodeI32 ( bytes );
    bytes += sizeof ( fudge_i32 );
    details->house_number = FudgeCodec_decodeI16 ( bytes );
    bytes += sizeof ( fudge_i16 );

    /* Copy in the string components and advance the source bytes pointer */
    memcpy ( details->street_name, bytes, ADDRESSDETAILS_NAME_FIELD_LEN );
    bytes += ADDRESSDETAILS_NAME_FIELD_LEN;
    memcpy ( details->city, bytes, ADDRESSDETAILS_CITY_FIELD_LEN );
    bytes += ADDRESSDETAILS_CITY_FIELD_LEN;
    memcpy ( details->postal_code, bytes, ADDRESSDETAILS_POST_FIELD_LEN );
    bytes += ADDRESSDETAILS_POST_FIELD_LEN;

    return FUDGE_OK;
}

/* Type coercing function for AddressDetails. Required, but can simply refuse
   to coerce the value (returning either FUDGE_COERCION_NOT_REQUIRED if the
   types match or FUDGE_INVALID_TYPE_COERCION if not).
   For this example a simple string conversion is supported. */
FudgeStatus FudgeType_coerceAddressDetails ( const FudgeField * source,
                                             const fudge_type_id type,
                                             FudgeFieldData * target,
                                             fudge_i32 * numbytes )
{
    /* Used for string output */
    static const char * statusString [ 3 ] = { "Unknown", "Active", "Past" };
    size_t statusInt;

    switch ( type )
    {
        case FUDGE_TYPE_ADDRESSDETAILS:
            return FUDGE_COERCION_NOT_REQUIRED;
        case FUDGE_TYPE_STRING:
        {
            FudgeStatus status;
            char * tempstr;
            const AddressDetails * details = ( const AddressDetails * ) source->data.bytes;
            int stringlen;

            /* Calculate how much space will be needed for the string and set
               the return value */
            stringlen = ADDRESSDETAILS_NAME_FIELD_LEN +
                        ADDRESSDETAILS_CITY_FIELD_LEN +
                        ADDRESSDETAILS_POST_FIELD_LEN + 32;

            /* Allocate string space in temporary buffer */
            if ( ! ( tempstr = ( char * ) malloc ( stringlen ) ) )
                return FUDGE_OUT_OF_MEMORY;

            /* Encode string - yes snprintf is better, but it's not portable
               enough for the example code. */
            statusInt = ( size_t ) ( details->status >= 0 && details->status <= 3 ? details->status
                                                                                  : Status_Unknown );
            sprintf ( tempstr,
                      "[%s] %d %s, %s. %s.",
                      statusString [ statusInt ],
                      details->house_number,
                      details->street_name,
                      details->city,
                      details->postal_code );

            /* Use the temporary string to create the return value FudgeString */
            status = FudgeString_createFromASCIIZ ( &( target->string ), tempstr );
            free ( tempstr );
            *numbytes = FudgeString_getSize ( target->string );
            return status;
        }
        default:
            return FUDGE_INVALID_TYPE_COERCION;
    }
}

void fatalFudgeError ( FudgeStatus status, const char * context );
AddressDetails * constructAddressDetails ( AddressStatus status,
                                           fudge_i16 number,
                                           const char * street,
                                           const char * city,
                                           const char * postal );

int main ( int argc, char * argv [ ] )
{
    FudgeStatus status;
    FudgeMsg message;
    FudgeMsgEnvelope envelope;
    AddressDetails * details [ 2 ];
    fudge_i16 ordinal;
    fudge_byte * encoded;
    fudge_i32 encodedsize;

    /* Initialise the Fudge library */
    if ( ( status = Fudge_init ( ) ) )
        fatalFudgeError ( status, "Failed to initialise Fudge library" );

    /* Register the AddressDetails type */
    if ( ( status = FudgeRegistry_registerType ( FUDGE_TYPE_ADDRESSDETAILS,
                                                 FUDGE_TYPE_PAYLOAD_BYTES,
                                                 FudgeCodec_decodeFieldAddressDetails,
                                                 FudgeCodec_encodeFieldAddressDetails,
                                                 FudgeType_coerceAddressDetails ) ) )
        fatalFudgeError ( status, "Failed to register AddressDetails type" );

    /* Construct and encode two address details */
    details [ 0 ] = constructAddressDetails ( Status_Past, 123, "Fake St.", "Some City", "P05 T4L" );
    details [ 1 ] = constructAddressDetails ( Status_Active, 45, "Faux Road", "Some Town", "FUD 63C" );

    /* Create a message and add the two details */
    if ( ( status = FudgeMsg_create ( &message ) ) )
        fatalFudgeError ( status, "Failed to create Fudge message" );

    for ( ordinal = 0; ordinal < 2; ++ordinal )
        FudgeMsg_addFieldAddressDetails ( message, 0, &ordinal, details [ ordinal ] );

    /* Encode the message */
    if ( ( status = FudgeMsgEnvelope_create ( &envelope, 0, 0, 0, message ) ) )
        fatalFudgeError ( status, "Failed to create Fudge messag envelope" );

    if ( ( status = FudgeCodec_encodeMsg ( envelope, &encoded, &encodedsize ) ) )
        fatalFudgeError ( status, "Failed to encode Fudge message" );

    /* Clean up source details and messge */
    free ( details [ 0 ] );
    free ( details [ 1 ] );
    FudgeMsgEnvelope_release ( envelope );
    FudgeMsg_release ( message );

    /* Decode the message and release the encoded bytes array */
    if ( ( status = FudgeCodec_decodeMsg ( &envelope, encoded, encodedsize ) ) )
        fatalFudgeError ( status, "Failed to decode Fudge message" );
    free ( encoded );

    /* Retrieve, convert and display the fields */
    for ( ordinal = 0; ordinal < 2; ++ordinal )
    {
        FudgeField field;
        FudgeFieldData data;
        FudgeTypePayload payload;
        fudge_i32 datasize;
        char * ascii;

        if ( ( status = FudgeMsg_getFieldByOrdinal ( &field, FudgeMsgEnvelope_getMessage ( envelope ), ordinal ) ) )
            fatalFudgeError ( status, "Failed to find field" );

        /* Convert the field in to a string */
        if ( ( status = FudgeMsg_getFieldAs ( &field, FUDGE_TYPE_STRING, &data, &payload, &datasize ) ) )
            fatalFudgeError ( status, "Failed to convert field to string" );

        /* This is a bit paranoid, but it's checking that the string
           conversion actually resulted in string payload */
        if ( payload != FUDGE_TYPE_PAYLOAD_STRING )
        {
            fprintf ( stderr, "FATAL ERROR: Retrieving field %d as a string returned a non-string!\n", ordinal );
            exit ( 1 );
        }

        FudgeString_convertToASCIIZ ( &ascii, data.string );
        printf ( "Field %d: %s\n", ordinal, ascii );
        free ( ascii );
        FudgeString_release ( data.string );
    }

    /* Clean up */
    FudgeMsgEnvelope_release ( envelope );
    return 0;
}

void fatalFudgeError ( FudgeStatus status, const char * context )
{
    fprintf ( stderr, "FATAL ERROR: %s : %s\n", context, FudgeStatus_strerror ( status ) );
    exit ( 1 );
}

AddressDetails * constructAddressDetails ( AddressStatus status,
                                           fudge_i16 number,
                                           const char * street,
                                           const char * city,
                                           const char * postal )
{
    AddressDetails * details = ( AddressDetails * ) malloc ( sizeof ( AddressDetails ) );
    details->status = status;
    details->house_number = number;
    strncpy ( details->street_name, street, ADDRESSDETAILS_NAME_FIELD_LEN );
    strncpy ( details->city, city, ADDRESSDETAILS_CITY_FIELD_LEN );
    strncpy ( details->postal_code, postal, ADDRESSDETAILS_POST_FIELD_LEN );
    return details;
}

