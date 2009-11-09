#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>

typedef enum
{
    /* Fixed length types */
    FUDGE_TYPE_INDICATOR        = 0,
    FUDGE_TYPE_BOOLEAN          = 1,    /* Stored as 8-bit integer      */
    FUDGE_TYPE_BYTE             = 2,    /* Signed 8-bit integer         */
    FUDGE_TYPE_SHORT            = 3,    /* Signed 16-bit integer        */
    FUDGE_TYPE_INT              = 4,    /* Signed 32-bit integer        */
    FUDGE_TYPE_LONG             = 5,    /* Signed 64-bit integer        */
    FUDGE_TYPE_FLOAT            = 10,   /* 32-bit floating point        */
    FUDGE_TYPE_DOUBLE           = 11,   /* 64-bit floating point        */
    FUDGE_TYPE_FUDGE_MSG        = 15,   /* Indicates a sub-message      */

    /* Variable length types */
    FUDGE_TYPE_BYTE_ARRAY       = 6,
    FUDGE_TYPE_SHORT_ARRAY      = 7,
    FUDGE_TYPE_INT_ARRAY        = 8,
    FUDGE_TYPE_LONG_ARRAY       = 9,
    FUDGE_TYPE_FLOAT_ARRAY      = 12,
    FUDGE_TYPE_DOUBLE_ARRAY     = 13,
    FUDGE_TYPE_STRING           = 14,   /* UTF-8 encoded string         */

    /* Fixed length byte array types */
    FUDGE_TYPE_BYTE_ARRAY_4     = 17,   /* Byte array with 4 elements   */
    FUDGE_TYPE_BYTE_ARRAY_8     = 18,   /* Byte array with 8 elements   */
    FUDGE_TYPE_BYTE_ARRAY_16    = 19,   /* Byte array with 16 elements  */
    FUDGE_TYPE_BYTE_ARRAY_20    = 20,   /* Byte array with 20 elements  */
    FUDGE_TYPE_BYTE_ARRAY_32    = 21,   /* Byte array with 32 elements  */
    FUDGE_TYPE_BYTE_ARRAY_64    = 22,   /* Byte array with 64 elements  */
    FUDGE_TYPE_BYTE_ARRAY_128   = 23,   /* Byte array with 128 elements */
    FUDGE_TYPE_BYTE_ARRAY_256   = 24,   /* Byte array with 256 elements */
    FUDGE_TYPE_BYTE_ARRAY_512   = 25    /* Byte array with 512 elements */

} FudgeTypeId;


typedef struct
{
    FudgeTypeId typeId;
    int isVariableSize;
    int fixedSize;
} FudgeFieldType;

FudgeFieldType * FudgeFieldType_create ( FudgeTypeId typeId,
                                         int isVariableSize,
                                         int fixedSize );

int FudgeFieldType_dumpToFile ( FudgeFieldType * type, FILE * file );


typedef struct FudgeTypeDictionary * FudgeTypeDictionaryRef;

FudgeTypeDictionaryRef FudgeTypeDictionary_create ( );
void FudgeTypeDictionary_destroy ( FudgeTypeDictionaryRef dictionary );

int FudgeTypeDictionary_addType ( FudgeTypeDictionaryRef dictionary, FudgeFieldType * type );

int FudgeTypeDictionary_dumpToFile ( FudgeTypeDictionaryRef dictionary, FILE * file );

/*
   TODO Create FudgeTypeDictionary storing type information in an easily referenced manner (rb-tree?)
   TODO Add functionality for FudgeTypeDictionary:
   TODO     - getByTypeId
*/

#endif

