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
#ifndef INC_FUDGE_STRING_H
#define INC_FUDGE_STRING_H

#include "fudge/status.h"
#include "fudge/types.h"

#ifdef __cplusplus
    extern "C" {
#endif

/* The FudgeString provides a thread-safe (but not reentrant), reference
   counted, immutable string implementation. A FudgeString can be created from
   a number of different source data types, but the string is always stored as
   (non-modified) UTF8. For valid unicode sequences the conversion process  is
   loss-less. For example: creating an instance from UTF32 data and then
   retrieving the data as a UTF32 string will yield the data as the source.
   This applies to all unicode creation and conversion functions.

   Due to limitations of the format, converting to ASCII will only be loss-
   less if the source data was also ASCII. If the source was unicode data,
   non-ASCII characters will be rendered as question marks.

   The string comparison logic is a simple bytes comparison, with one
   exception: byte-order markers are skipped. The return value follows the
   same rules as strcmp.

   All strings are created with a reference count of one. To release the
   memory held by a string it must be released. To avoid memory leaks or
   segmentation faults the number of release operations per string must
   be equal to one more than the number of retain operations.

   Thread safety:

   The FudgeString API is thread safe: multiple threads can create, retain,
   convert, compare and release strings concurrently. As the FudgeString
   instances are immutable, multiple threads can get, convert and compare
   a single FudgeString instance. However, multiple threads must NOT retain
   or release a single thread instance in parallel. The reference counting
   logic/state is not reentrant and external locking should be used to
   protect these operations if the string is to be used in this way.
*/

FUDGEAPI FudgeStatus FudgeString_createFromASCII ( FudgeString * string, const char * chars, size_t numchars );
FUDGEAPI FudgeStatus FudgeString_createFromASCIIZ ( FudgeString * string, const char * chars );
FUDGEAPI FudgeStatus FudgeString_createFromUTF8 ( FudgeString * string, const fudge_byte * bytes, size_t numbytes );
FUDGEAPI FudgeStatus FudgeString_createFromUTF16 ( FudgeString * string, const fudge_byte * bytes, size_t numbytes );
FUDGEAPI FudgeStatus FudgeString_createFromUTF32 ( FudgeString * string, const fudge_byte * bytes, size_t numbytes );

FUDGEAPI FudgeStatus FudgeString_retain ( FudgeString string );
FUDGEAPI FudgeStatus FudgeString_release ( FudgeString string );

FUDGEAPI size_t FudgeString_getSize ( const FudgeString string );
FUDGEAPI const fudge_byte * FudgeString_getData ( const FudgeString string );

FUDGEAPI size_t FudgeString_getLength ( const FudgeString string );

FUDGEAPI FudgeStatus FudgeString_convertToASCIIZ ( char * * target, const FudgeString string );
FUDGEAPI FudgeStatus FudgeString_convertToUTF16 ( fudge_byte * * target, size_t * numbytes, const FudgeString string );
FUDGEAPI FudgeStatus FudgeString_convertToUTF32 ( fudge_byte * * target, size_t * numbytes, const FudgeString string );

FUDGEAPI size_t FudgeString_copyToASCII ( char * buffer, size_t buffersize, const FudgeString string );
FUDGEAPI size_t FudgeString_copyToUTF16 ( fudge_byte * buffer, size_t buffersize, const FudgeString string );
FUDGEAPI size_t FudgeString_copyToUTF32 ( fudge_byte * buffer, size_t buffersize, const FudgeString string );

FUDGEAPI int FudgeString_compare ( const FudgeString left, const FudgeString right );

#pragma pack(push,8)
typedef struct {
  void * reserved;
  const char * bytes;
  size_t numbytes;
} FudgeStringStatic;
#pragma pack(pop)

FudgeString FudgeString_fromStatic (FudgeStringStatic * string);

#ifdef __cplusplus
    }
#endif

#endif

