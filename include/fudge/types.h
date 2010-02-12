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
#ifndef INC_FUDGE_TYPES_H
#define INC_FUDGE_TYPES_H

#include "fudge/platform.h"

#ifdef __cplusplus
    extern "C" {
#endif

/*****************************************************************************
 * Primitive type definitions
 */

/* Bitmap values used to indicate the presence of optional values in the
   FudgeField type. */
typedef enum
{
    FUDGE_FIELD_HAS_NAME    = 0x1,
    FUDGE_FIELD_HAS_ORDINAL = 0x2
} FudgeFieldFlags;

/* Custom boolean definition: matches the standard definitions, in that it's
   the same length as an int and is false if zero. */
typedef enum
{
    FUDGE_FALSE = 0x0,
    FUDGE_TRUE  = 0x1
} fudge_bool;

/* Create types of the correct size. Currently dependent on C99 definitions
   but could be replaced with architecture specific types, just as long as
   they're wide enough to hold the values. */
typedef int8_t  fudge_byte;
typedef int16_t fudge_i16;
typedef int32_t fudge_i32;
typedef int64_t fudge_i64;
typedef float   fudge_f32;
typedef double  fudge_f64;

typedef uint8_t fudge_type_id;

/*****************************************************************************
 * Structure/union type definitions
 */

typedef struct FudgeMsgImpl * FudgeMsg;

typedef struct FudgeStringImpl * FudgeString;

typedef struct
{
    fudge_byte directives;      /* Processing directives, for future use */
    fudge_byte schemaversion;   /* Application specific Schema version */
    fudge_i16 taxonomy;         /* Application specific taxonomy reference */
    FudgeMsg message;           /* The actual message */
} FudgeMsgEnvelope;

typedef union
{
    /* Primitive types */
    fudge_bool boolean;
    fudge_byte byte;
    fudge_i16 i16;
    fudge_i32 i32;
    fudge_i64 i64;
    fudge_f32 f32;
    fudge_f64 f64;

    /* Embedded message */
    FudgeMsg message;

    /* Other data types */
    const fudge_byte * bytes;

} FudgeFieldData;

typedef struct
{
    fudge_type_id type;     /* The field type: used to interpret the data union */
    fudge_i32 numbytes;     /* The size of the bytes array in the data union */
    FudgeFieldData data;    /* Union holding the field data */
    FudgeFieldFlags flags;  /* Indicates the presence of optional values */
    fudge_byte * name;      /* The name of the field, can be null */
    fudge_i32 namelen;      /* Number of bytes in the name, zero if name is null */
    fudge_i16 ordinal;      /* The field ordinal, or zero if unset */
} FudgeField;

/*****************************************************************************
 * Built-in Fudge type identifiers (all of type fudge_type_id)
 */

/* Fixed length types */
#define FUDGE_TYPE_INDICATOR        0 
#define FUDGE_TYPE_BOOLEAN          1     /* Stored as 8-bit integer      */
#define FUDGE_TYPE_BYTE             2     /* Signed 8-bit integer         */
#define FUDGE_TYPE_SHORT            3     /* Signed 16-bit integer        */
#define FUDGE_TYPE_INT              4     /* Signed 32-bit integer        */
#define FUDGE_TYPE_LONG             5     /* Signed 64-bit integer        */
#define FUDGE_TYPE_FLOAT            10    /* 32-bit floating point        */
#define FUDGE_TYPE_DOUBLE           11    /* 64-bit floating point        */

/* Variable length types */
#define FUDGE_TYPE_BYTE_ARRAY       6 
#define FUDGE_TYPE_SHORT_ARRAY      7 
#define FUDGE_TYPE_INT_ARRAY        8 
#define FUDGE_TYPE_LONG_ARRAY       9 
#define FUDGE_TYPE_FLOAT_ARRAY      12 
#define FUDGE_TYPE_DOUBLE_ARRAY     13 
#define FUDGE_TYPE_STRING           14    /* UTF-8 encoded string         */
#define FUDGE_TYPE_FUDGE_MSG        15    /* Indicates a sub-message      */

/* Fixed length byte array types */
#define FUDGE_TYPE_BYTE_ARRAY_4     17    /* Byte array with 4 elements   */
#define FUDGE_TYPE_BYTE_ARRAY_8     18    /* Byte array with 8 elements   */
#define FUDGE_TYPE_BYTE_ARRAY_16    19    /* Byte array with 16 elements  */
#define FUDGE_TYPE_BYTE_ARRAY_20    20    /* Byte array with 20 elements  */
#define FUDGE_TYPE_BYTE_ARRAY_32    21    /* Byte array with 32 elements  */
#define FUDGE_TYPE_BYTE_ARRAY_64    22    /* Byte array with 64 elements  */
#define FUDGE_TYPE_BYTE_ARRAY_128   23    /* Byte array with 128 elements */
#define FUDGE_TYPE_BYTE_ARRAY_256   24    /* Byte array with 256 elements */
#define FUDGE_TYPE_BYTE_ARRAY_512   25    /* Byte array with 512 elements */

/*****************************************************************************
 * Type information functions
 */

/* Returns true if the type is fixed width, unknown types are always of
   variable width. */
FUDGEAPI fudge_bool FudgeType_typeIsFixedWidth ( fudge_type_id type );

/* Returns the width of the type (in bytes), if it is fixed width. Otherwise
   returns -1. */
FUDGEAPI fudge_i32 FudgeType_getFixedWidth ( fudge_type_id type );

#ifdef __cplusplus
    }
#endif

#endif

