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

/* Output functions */
void outputEnvelope ( FudgeMsgEnvelope envelope );
void outputMessage ( FudgeMsg message, unsigned int indent );
void outputField ( FudgeField * field, unsigned int indent );
void outputString ( const FudgeString string );
void outputArray ( const fudge_byte * byte, fudge_i32 numbytes, const char * format, fudge_i32 width, fudge_i32 truncate );
void outputType ( fudge_type_id type );
void outputIndent ( unsigned int indent );

/* Other convenience functions */
void displayUsage ( );
void fatalError ( const char * error );
void fatalFudgeError ( FudgeStatus status, const char * context );
void loadFile ( fudge_byte * * bytes, fudge_i32 * numbytes, const char * filename );
void appendBytes ( fudge_byte * * target, fudge_i32 * targetsize, const fudge_byte * source, fudge_i32 sourcesize );

/* For the most basic example of Fudge-C use, see the "simple.c" file that
   should be in the same directory as this one.

   This is a basic "pretty printer" for Fudge-C encoded message files. It
   takes the filename of the message to display and dumps the contents, in
   human readable form, to the standard output.

   Rather than assume a Unicode compatible console, all strings are
   converted in to 7bit ASCII before being output.
*/
int main ( int argc, char * argv [ ] )
{
    FudgeStatus status;
    FudgeMsgEnvelope envelope;
    const char * filename;
    fudge_byte * bytes;
    fudge_i32 numbytes;

    /* Get the filename from the command-line arguments */
    if ( argc != 2 )
        displayUsage ( );
    filename = argv [ 1 ];

    /* Initialise the Fudge library */
    if ( ( status = Fudge_init ( ) ) )
        fatalFudgeError ( status, "Failed to initialise Fudge library" );

    /* Load in the file and attempt to decode it */
    loadFile ( &bytes, &numbytes, filename );
    if ( ( status = FudgeCodec_decodeMsg ( &envelope, bytes, numbytes ) ) )
        fatalFudgeError ( status, "Failed to decode file" );

    /* Output the file contents */
    outputEnvelope ( envelope );
    FudgeMsg_release ( envelope.message );
    return 0;
}

void outputEnvelope ( FudgeMsgEnvelope envelope )
{
    printf ( "Schema version: %d\nTaxonomy: %d\nMessage:\n{\n",
             ( int ) envelope.schemaversion,
             ( int ) envelope.taxonomy );
    outputMessage ( envelope.message, 0 );
    printf ( "}\n" );
}

void outputMessage ( FudgeMsg message, unsigned int indent )
{
    fudge_i32 index, numfields;
    FudgeField * fields;
    
    /* Get the number of fields in the message and allocate enough space to
       hold them. Bail out if the message has no fields. */
    if ( ! ( numfields = FudgeMsg_numFields ( message ) ) )
        return;
    if ( ! ( fields = ( FudgeField * ) malloc ( numfields * sizeof ( FudgeField ) ) ) )
        fatalError ( "Unable to allocate memory for field array" );

    /* Retrieve all of fields in the message and output them in turn */
    numfields = FudgeMsg_getFields ( fields, numfields, message );
    for ( index = 0; index < numfields; ++index )
        outputField ( &( fields [ index ] ), indent + 1 );

    /* While the field contents are owned by the message, the array used to
       hold them is the property of the local scope. */
    free ( fields );
}

void outputField ( FudgeField * field, unsigned int indent )
{
    /* Output the field's type, name (if present) and ordinal (if present) */
    outputIndent ( indent );
    outputType ( field->type );
    printf ( " " );
    if ( field->flags & FUDGE_FIELD_HAS_NAME )
    {   
        /* TODO Replace when field names become FudgeString */
        FudgeString temp;
        FudgeString_createFromUTF8 ( &temp, field->name, field->namelen );
        outputString ( temp );
        FudgeString_release ( temp );

        if ( field->flags & FUDGE_FIELD_HAS_ORDINAL )
            printf ( "/" );
    }
    if ( field->flags & FUDGE_FIELD_HAS_ORDINAL )
        printf ( "ord(%d)", field->ordinal );
    printf ( ": " );

    /* Output the field contents */
    switch ( field->type )
    {
        case FUDGE_TYPE_INDICATOR:      break;
        case FUDGE_TYPE_BOOLEAN:        printf ( field->data.boolean ? "true" : "false" ); break;
        case FUDGE_TYPE_BYTE:           printf ( "%d", field->data.byte ); break;
        case FUDGE_TYPE_SHORT:          printf ( "%d", field->data.i16 ); break;
        case FUDGE_TYPE_INT:            printf ( "%d", field->data.i32 ); break;
        case FUDGE_TYPE_LONG:           printf ( "%ld", ( long int ) field->data.i64 ); break;
        case FUDGE_TYPE_FLOAT:          printf ( "%f", field->data.f32 ); break;
        case FUDGE_TYPE_DOUBLE:         printf ( "%f", field->data.f64 ); break;
        case FUDGE_TYPE_SHORT_ARRAY:    outputArray ( field->data.bytes, field->numbytes, "%d", 2, 8 ); break;
        case FUDGE_TYPE_INT_ARRAY:      outputArray ( field->data.bytes, field->numbytes, "%d", 4, 8 ); break;
        case FUDGE_TYPE_LONG_ARRAY:     outputArray ( field->data.bytes, field->numbytes, "%lu", 8, 4 ); break;
        case FUDGE_TYPE_FLOAT_ARRAY:    outputArray ( field->data.bytes, field->numbytes, "%f", 4, 4 ); break;
        case FUDGE_TYPE_DOUBLE_ARRAY:   outputArray ( field->data.bytes, field->numbytes, "%f", 8, 4 ); break;
        case FUDGE_TYPE_STRING:         outputString ( field->data.string ); break;
        case FUDGE_TYPE_BYTE_ARRAY: 
        case FUDGE_TYPE_BYTE_ARRAY_4:
        case FUDGE_TYPE_BYTE_ARRAY_8:
        case FUDGE_TYPE_BYTE_ARRAY_16:
        case FUDGE_TYPE_BYTE_ARRAY_20:
        case FUDGE_TYPE_BYTE_ARRAY_32:
        case FUDGE_TYPE_BYTE_ARRAY_64:
        case FUDGE_TYPE_BYTE_ARRAY_128:
        case FUDGE_TYPE_BYTE_ARRAY_256:
        case FUDGE_TYPE_BYTE_ARRAY_512:
            outputArray ( field->data.bytes, field->numbytes, "%d", 1, 10 ); 
            break;
        case FUDGE_TYPE_FUDGE_MSG:
            printf ( "\n" );
            outputIndent ( indent );
            printf ( "{\n" );
            outputMessage ( field->data.message, indent + 1 );
            outputIndent ( indent );
            printf ( "}" );
            break;
        default:
            printf ( "%d bytes ", field->numbytes );
            outputArray ( field->data.bytes, field->numbytes, "%d", 1, 8 ); 
            break;
    }

    printf ( "\n" );
}

void outputString ( const FudgeString string )
{
    char * ascii;
    FudgeString_convertToASCIIZ ( &ascii, string );
    printf ( "\"%s\"", ascii );
    free ( ascii );
}

void outputArray ( const fudge_byte * byte, fudge_i32 numbytes, const char * format, fudge_i32 width, fudge_i32 truncate )
{
    fudge_i32 index, numelements = numbytes / width;
    
    if ( truncate > numelements )
        truncate = numelements;

    /* Output the first few elements of the array */
    printf ( "[ " );
    for ( index = 0; index < truncate; ++index )
    {
        printf ( format, *( byte + ( index * width ) ) );
        if ( index + 1 < numelements )
            printf ( ", " );
    }

    /* If the array is longer than that output, add a suffix */
    if ( truncate < numelements )
        printf ( "... %d more", numelements - truncate );
    printf ( " ]" );
}

void outputType ( fudge_type_id type )
{
    static const int byteArraySizes [] = { 4, 8, 16, 20, 32, 64, 128, 256, 512 };

    switch ( type )
    {
        case FUDGE_TYPE_INDICATOR:      printf ( "indicator" ); break;
        case FUDGE_TYPE_BOOLEAN:        printf ( "boolean" );   break;
        case FUDGE_TYPE_BYTE:           printf ( "byte" );      break;
        case FUDGE_TYPE_SHORT:          printf ( "short" );     break;
        case FUDGE_TYPE_INT:            printf ( "int" );       break;
        case FUDGE_TYPE_LONG:           printf ( "long" );      break;
        case FUDGE_TYPE_FLOAT:          printf ( "float" );     break;
        case FUDGE_TYPE_DOUBLE:         printf ( "double" );    break;
        case FUDGE_TYPE_BYTE_ARRAY:     printf ( "byte[]" );    break;
        case FUDGE_TYPE_SHORT_ARRAY:    printf ( "short[]" );   break;
        case FUDGE_TYPE_INT_ARRAY:      printf ( "int[]" );     break;
        case FUDGE_TYPE_LONG_ARRAY:     printf ( "long[]" );    break;
        case FUDGE_TYPE_FLOAT_ARRAY:    printf ( "float[]" );   break;
        case FUDGE_TYPE_DOUBLE_ARRAY:   printf ( "double[]" );  break;
        case FUDGE_TYPE_STRING:         printf ( "string" );    break;
        case FUDGE_TYPE_FUDGE_MSG:      printf ( "message" );   break;
        case FUDGE_TYPE_BYTE_ARRAY_4:
        case FUDGE_TYPE_BYTE_ARRAY_8:
        case FUDGE_TYPE_BYTE_ARRAY_16:
        case FUDGE_TYPE_BYTE_ARRAY_20:
        case FUDGE_TYPE_BYTE_ARRAY_32:
        case FUDGE_TYPE_BYTE_ARRAY_64:
        case FUDGE_TYPE_BYTE_ARRAY_128:
        case FUDGE_TYPE_BYTE_ARRAY_256:
        case FUDGE_TYPE_BYTE_ARRAY_512:
            printf ( "byte[%d]", byteArraySizes [ type - FUDGE_TYPE_BYTE_ARRAY_4 ] );
            break;
        default:
            printf ( "unknown(%d)", ( int ) type );
            break;
    }
}

void outputIndent ( unsigned int indent )
{
    for ( ; indent; --indent )
        printf ( "  " );
}

void displayUsage ( )
{
    fprintf ( stderr, "Usage: prettyprint FUDGE_MESSAGE_FILE\n\n" );
    exit ( 0 );
}

void fatalError ( const char * error )
{
    fprintf ( stderr, "FATAL ERROR: %s\n", error );
    exit ( 1 );
}

void fatalFudgeError ( FudgeStatus status, const char * context )
{
    fprintf ( stderr, "FATAL ERROR: %s : %s\n",
                      context,
                      FudgeStatus_strerror ( status ) );
    exit ( 1 );
}

void loadFile ( fudge_byte * * bytes, fudge_i32 * numbytes, const char * filename )
{
    FILE * file;
    fudge_byte buffer [ 1024 ];

    /* Load in the file in 1K blocks */
    if ( ! ( file = fopen ( filename, "rb" ) ) )
        fatalError ( "Failed to open input file" );
    *numbytes = 0;
    while ( ! feof ( file ) )
    {
        fudge_i32 bytesread = fread ( buffer, 1, sizeof ( buffer ), file );
        if ( bytesread )
            appendBytes ( bytes, numbytes, buffer, bytesread );
    }
    fclose ( file );
}

void appendBytes ( fudge_byte * * target, fudge_i32 * targetsize, const fudge_byte * source, fudge_i32 sourcesize )
{
    if ( *targetsize )
    {
        /* Target array exists: create a new one large enough for the current
           data and the new data, then copy across the current data. */
        fudge_byte * current = *target;
        if ( ! ( *target = ( fudge_byte * ) malloc ( sourcesize + *targetsize ) ) )
            fatalError ( "Unable to extend memory for file input array" );
        memcpy ( *target, current, *targetsize );
        free ( current );
    }
    else
    {
        /* Target array doesn't exist: create one large enough for the new
           data */
        if ( ! ( *target = ( fudge_byte * ) malloc ( sourcesize ) ) )
            fatalError ( "Unable to allocate memory for file input array" );
    }

    /* Append the new data */
    memcpy ( *target + *targetsize, source, sourcesize );
    *targetsize += sourcesize;
}

