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
#include "fudge/string.h"
#include "convertutf.h"
#include "simpletest.h"

/* Source strings - converted using iconv */
static const fudge_byte StringTest_utf8Source []  = { 0xe2, 0x80, 0xbc, 0x20, 0xe2, 0x88, 0x9a, 0xe2,
                                                      0x84, 0xa6, 0x54, 0x65, 0x73, 0x74, 0xe2, 0x88,
                                                      0x91, 0xd1, 0x84 };
static const fudge_byte StringTest_utf16Source [] = { 0xff, 0xfe, 0x3c, 0x20, 0x20, 0x00, 0x1a, 0x22,
                                                      0x26, 0x21, 0x54, 0x00, 0x65, 0x00, 0x73, 0x00,
                                                      0x74, 0x00, 0x11, 0x22, 0x44, 0x04 };
static const fudge_byte StringTest_utf32Source [] = { 0xff, 0xfe, 0x00, 0x00, 0x3c, 0x20, 0x00, 0x00,
                                                      0x20, 0x00, 0x00, 0x00, 0x1a, 0x22, 0x00, 0x00,
                                                      0x26, 0x21, 0x00, 0x00, 0x54, 0x00, 0x00, 0x00,
                                                      0x65, 0x00, 0x00, 0x00, 0x73, 0x00, 0x00, 0x00,
                                                      0x74, 0x00, 0x00, 0x00, 0x11, 0x22, 0x00, 0x00,
                                                      0x44, 0x04, 0x00, 0x00 };

/* UTF8 version of the UTF16/32 source strings. These contained a BOM at the
   start of the strings which should be present in the UTF8 version. */
static const fudge_byte StringTest_utf8Converted [] =  { 0xef, 0xbb, 0xbf, 0xe2, 0x80, 0xbc, 0x20, 0xe2,
                                                         0x88, 0x9a, 0xe2, 0x84, 0xa6, 0x54, 0x65, 0x73,
                                                         0x74, 0xe2, 0x88, 0x91, 0xd1, 0x84 };

/* ASCII version of the source strings */
static const fudge_byte StringTest_asciiConverted [] = { 0x3f, 0x20, 0x3f, 0x3f, 0x54, 0x65, 0x73, 0x74,
                                                         0x3f, 0x3f };

DEFINE_TEST( CreateFromASCII )
    FudgeString one, two;

    /* Test NULL failure cases */
    TEST_EQUALS_INT( FudgeString_createFromASCII ( &one, 0, 1 ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeString_createFromASCIIZ ( 0, "String One" ), FUDGE_NULL_POINTER );

    /* Construct strings and check content */
    TEST_EQUALS_INT( FudgeString_createFromASCII ( &one, "String One", 10 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeString_createFromASCIIZ ( &two, "String Two" ), FUDGE_OK );

    TEST_EQUALS_MEMORY( FudgeString_getData ( one ), FudgeString_getSize ( one ), "String One", 10 );
    TEST_EQUALS_MEMORY( FudgeString_getData ( two ), FudgeString_getSize ( two ), "String Two", 10 );

    /* Test comparisons */
    TEST_EQUALS_INT( FudgeString_compare ( one, two ), -1 );
    TEST_EQUALS_INT( FudgeString_compare ( two, one ), 1 );
    TEST_EQUALS_INT( FudgeString_compare ( one, one ), 0 );
    TEST_EQUALS_INT( FudgeString_compare ( two, two ), 0 );

    /* Release remaining references */
    TEST_EQUALS_INT( FudgeString_release ( one ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeString_release ( two ), FUDGE_OK );

    /* Release failure case */
    TEST_EQUALS_INT( FudgeString_release ( 0 ), FUDGE_NULL_POINTER );
END_TEST

DEFINE_TEST( CreateFromUTF8 )
    FudgeString string;
    char * buffer;
    fudge_byte * bytes;
    size_t numbytes;
    
    TEST_EQUALS_INT( FudgeString_createFromUTF8 ( &string,
                                                  StringTest_utf8Source,
                                                  sizeof ( StringTest_utf8Source ) ), FUDGE_OK );

    /* Check the string contents */
    TEST_EQUALS_MEMORY( FudgeString_getData ( string ), FudgeString_getSize ( string ), StringTest_utf8Source, sizeof ( StringTest_utf8Source ) );

    /* Test ASCII output */
    TEST_EQUALS_INT( FudgeString_convertToASCIIZ ( &buffer, string ), FUDGE_OK );
    TEST_EQUALS_MEMORY( buffer, strlen ( buffer ), StringTest_asciiConverted, sizeof ( StringTest_asciiConverted ) );
    free ( buffer );

    /* Test UTF16 output. Note that as the source string had no BOM, neither
       will the output string; hence the need to jump the first two bytes in
       the UTF16 comparison string. */
    TEST_EQUALS_INT( FudgeString_convertToUTF16 ( &bytes, &numbytes, string ), FUDGE_OK );
    TEST_EQUALS_MEMORY( bytes, numbytes, StringTest_utf16Source + 2, sizeof ( StringTest_utf16Source ) - 2 );
    free ( bytes );

    /* Test UTF32 output. As for the UTF16 output test there is no BOM in the
       source string, so the first four bytes of the UTF32 comparison string
       must be skipped. */
    TEST_EQUALS_INT( FudgeString_convertToUTF32 ( &bytes, &numbytes, string ), FUDGE_OK );
    TEST_EQUALS_MEMORY( bytes, numbytes, StringTest_utf32Source + 4, sizeof ( StringTest_utf32Source ) - 4 );
    free ( bytes );

    FudgeString_release ( string );
END_TEST

DEFINE_TEST( CreateFromUTF16 )
    FudgeString string;
    char * buffer;
    fudge_byte * bytes;
    size_t numbytes;
    
    TEST_EQUALS_INT( FudgeString_createFromUTF16 ( &string,
                                                   StringTest_utf16Source,
                                                   sizeof ( StringTest_utf16Source ) ), FUDGE_OK );
    
    /* Check the string contents */
    TEST_EQUALS_MEMORY( FudgeString_getData ( string ), FudgeString_getSize ( string ), StringTest_utf8Converted, sizeof ( StringTest_utf8Converted ) );

    /* Test ASCII output */
    TEST_EQUALS_INT( FudgeString_convertToASCIIZ ( &buffer, string ), FUDGE_OK );
    TEST_EQUALS_MEMORY( buffer, strlen ( buffer ), StringTest_asciiConverted, sizeof ( StringTest_asciiConverted ) );
    free ( buffer );

    /* Test UTF16 output */
    TEST_EQUALS_INT( FudgeString_convertToUTF16 ( &bytes, &numbytes, string ), FUDGE_OK );
    TEST_EQUALS_MEMORY( bytes, numbytes, StringTest_utf16Source, sizeof ( StringTest_utf16Source ) );
    free ( bytes );

    /* Test UTF32 output */
    TEST_EQUALS_INT( FudgeString_convertToUTF32 ( &bytes, &numbytes, string ), FUDGE_OK );
    TEST_EQUALS_MEMORY( bytes, numbytes, StringTest_utf32Source, sizeof ( StringTest_utf32Source ) );
    free ( bytes );

    FudgeString_release ( string );
END_TEST

DEFINE_TEST( CreateFromUTF32 )
    FudgeString string;
    char * buffer;
    fudge_byte * bytes;
    size_t numbytes;

    TEST_EQUALS_INT( FudgeString_createFromUTF32 ( &string,
                                                   StringTest_utf32Source,
                                                   sizeof ( StringTest_utf32Source ) ), FUDGE_OK );
    
    /* Check the string contents */
    TEST_EQUALS_MEMORY( FudgeString_getData ( string ), FudgeString_getSize ( string ), StringTest_utf8Converted, sizeof ( StringTest_utf8Converted ) );

    /* Test ASCII output */
    TEST_EQUALS_INT( FudgeString_convertToASCIIZ ( &buffer, string ), FUDGE_OK );
    TEST_EQUALS_MEMORY( buffer, strlen ( buffer ), StringTest_asciiConverted, sizeof ( StringTest_asciiConverted ) );
    free ( buffer );

    /* Test UTF16 output */
    TEST_EQUALS_INT( FudgeString_convertToUTF16 ( &bytes, &numbytes, string ), FUDGE_OK );
    TEST_EQUALS_MEMORY( bytes, numbytes, StringTest_utf16Source, sizeof ( StringTest_utf16Source ) );
    free ( bytes );

    /* Test UTF32 output */
    TEST_EQUALS_INT( FudgeString_convertToUTF32 ( &bytes, &numbytes, string ), FUDGE_OK );
    TEST_EQUALS_MEMORY( bytes, numbytes, StringTest_utf32Source, sizeof ( StringTest_utf32Source ) );
    free ( bytes );

    FudgeString_release ( string );
END_TEST

DEFINE_TEST( Comparison )
    FudgeString nullString,         /* Null pointer */
                utf8String,         /* From UTF8 test string */
                utf8Duplicate,      /* From same UTF8 test string as above */
                utf8Reference,      /* Pointer copy of utf8String + FudgeString_retain */
                utf16String,        /* From UTF16 source string (should be identical, bar 3-byte BOM) */
                utf16Truncated,     /* From UTF16 source string, but missing last character */
                lowString;          /* Very low (<0x20) bytes */

    /* Construct the test strings */
    TEST_EQUALS_INT( FudgeString_createFromUTF8 ( &utf8String, StringTest_utf8Source, sizeof ( StringTest_utf8Source ) ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeString_createFromUTF8 ( &utf8Duplicate, StringTest_utf8Source, sizeof ( StringTest_utf8Source ) ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeString_createFromUTF16 ( &utf16String, StringTest_utf16Source, sizeof ( StringTest_utf16Source ) ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeString_createFromUTF16 ( &utf16Truncated, StringTest_utf16Source, sizeof ( StringTest_utf16Source ) - 2 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeString_createFromASCIIZ ( &lowString, "\b\n\t\t " ), FUDGE_OK );

    /* Create null and reference test strings */
    nullString = 0;
    utf8Reference = utf8String;
    TEST_EQUALS_INT( FudgeString_retain ( utf8Reference ), FUDGE_OK );

    /* Test NULL pointer comparisons */
    TEST_EQUALS_INT( FudgeString_compare ( nullString, nullString ), 0 );
    TEST_EQUALS_INT( FudgeString_compare ( utf8String , nullString ), 1 );
    TEST_EQUALS_INT( FudgeString_compare ( nullString, utf8String ), -1 );

    /* Test identical strings */
    TEST_EQUALS_INT( FudgeString_compare ( utf8String, utf8String ), 0 );
    TEST_EQUALS_INT( FudgeString_compare ( utf8String, utf8Duplicate ), 0 );
    TEST_EQUALS_INT( FudgeString_compare ( utf8Reference, utf8Duplicate ), 0 );
    TEST_EQUALS_INT( FudgeString_compare ( utf16String, utf8String ), 0 );
    TEST_EQUALS_INT( FudgeString_compare ( utf16String, utf16String ), 0 );
    TEST_EQUALS_INT( FudgeString_compare ( lowString, lowString), 0 );

    /* Test differing strings */
    TEST_EQUALS_INT( FudgeString_compare ( lowString, utf8String ), -1 );
    TEST_EQUALS_INT( FudgeString_compare ( utf16String, lowString ), 1 );

    TEST_EQUALS_INT( FudgeString_compare ( utf8String, utf16Truncated ), 1 );
    TEST_EQUALS_INT( FudgeString_compare ( utf16Truncated, utf16String ), -1 );

    /* Clean up */
    FudgeString_release ( utf8String );
    FudgeString_release ( utf8Duplicate );
    FudgeString_release ( utf8Reference );
    FudgeString_release ( utf16String );
    FudgeString_release ( utf16Truncated );
    FudgeString_release ( lowString );
END_TEST


DEFINE_TEST_SUITE( String )
    REGISTER_TEST( CreateFromASCII )
    REGISTER_TEST( CreateFromUTF8 )
    REGISTER_TEST( CreateFromUTF16 )
    REGISTER_TEST( CreateFromUTF32 )
    REGISTER_TEST( Comparison )
END_TEST_SUITE
