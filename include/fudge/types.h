#ifndef INC_FUDGE_TYPES_H
#define INC_FUDGE_TYPES_H

#include "fudge/platform.h"

/*****************************************************************************
 * Primitive type definitions
 */

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

typedef fudge_byte fudge_type_id;

/*****************************************************************************
 * Structure/union type definitions
 */

typedef struct FudgeMsgImpl * FudgeMsg;

typedef union
{
    /* Primitive types */
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
    const char * name;      /* The name of the field, can be null */
    FudgeFieldData data;    /* Union holding the field data */
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
#define FUDGE_TYPE_FUDGE_MSG        15    /* Indicates a sub-message      */

/* Variable length types */
#define FUDGE_TYPE_BYTE_ARRAY       6 
#define FUDGE_TYPE_SHORT_ARRAY      7 
#define FUDGE_TYPE_INT_ARRAY        8 
#define FUDGE_TYPE_LONG_ARRAY       9 
#define FUDGE_TYPE_FLOAT_ARRAY      12 
#define FUDGE_TYPE_DOUBLE_ARRAY     13 
#define FUDGE_TYPE_STRING           14    /* UTF-8 encoded string         */

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

#endif

