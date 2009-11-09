#include "types.h"
#include <stdlib.h>

typedef struct FudgeTypeNode
{
    FudgeFieldType * type;
    struct FudgeTypeNode * left,
                         * right;
} FudgeTypeNode;

typedef struct FudgeTypeDictionary
{
    FudgeTypeNode * nodes;
} FudgeTypeDictionary;


FudgeTypeNode * FudgeTypeNode_create ( FudgeFieldType * type )
{
    FudgeTypeNode * node;

    if ( ! type ) return 0;   /* TODO Set errno */

    if ( ! ( node = ( FudgeTypeNode * ) malloc ( sizeof ( FudgeTypeNode ) ) ) )
        return 0;   /* TODO Set errno */

    node->type = type;
    node->left = node->right = 0;
    return node;
}

void FudgeTypeNode_destroy ( FudgeTypeNode * node )
{
    if ( node )
    {
        free ( node->type );
        FudgeTypeNode_destroy ( node->left );
        FudgeTypeNode_destroy ( node->right );
        free ( node );
    }
}

int FudgeTypeNode_dumpToFile ( FudgeTypeNode * node, FILE * file )
{
    int error;

    if ( node )
    {
        if ( ( error = FudgeTypeNode_dumpToFile ( node->right, file ) ) )
            return error;

        fprintf ( file, "    " );
        error = FudgeFieldType_dumpToFile ( node->type, file );
        fprintf ( file, "\n" );
        if ( error )
            return error;

        if ( ( error = FudgeTypeNode_dumpToFile ( node->left, file ) ) )
            return error;
    }
    return 0;
}


FudgeFieldType * FudgeFieldType_create ( FudgeTypeId typeId,
                                         int isVariableSize,
                                         int fixedSize )
{
    FudgeFieldType * type = ( FudgeFieldType * ) malloc ( sizeof ( FudgeFieldType ) );
    if ( type )
    {
        type->typeId = typeId;
        type->isVariableSize = isVariableSize;
        type->fixedSize = fixedSize;
    }
    return type;
}

int FudgeFieldType_dumpToFile ( FudgeFieldType * type, FILE * file )
{
    if ( ! type ) return -1;    /* TODO Set errno */
    if ( ! file ) return -1;    /* TODO Set errno */

    fprintf ( file, "FudgeFieldType<%d,", type->typeId );
    if ( type->isVariableSize )
        fprintf ( file, "variable" );
    else
        fprintf ( file, "fixed(%d)", type->fixedSize );
    fprintf ( file, ">" );
    return 0;
}


FudgeTypeDictionaryRef FudgeTypeDictionary_create ( )
{
    FudgeTypeDictionaryRef dictionary = ( FudgeTypeDictionaryRef ) malloc ( sizeof ( FudgeTypeDictionary ) );
    if ( ! dictionary )
        return 0;   /* TODO Set errno */
    dictionary->nodes = 0;

    /* Add the built in types */
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_INDICATOR,       0, 0 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_BOOLEAN,         0, 1 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_BYTE,            0, 1 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_SHORT,           0, 2 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_INT,             0, 4 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_LONG,            0, 8 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_FLOAT,           0, 4 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_DOUBLE,          0, 8 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_FUDGE_MSG,       0, 0 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_BYTE_ARRAY,      1, 0 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_SHORT_ARRAY,     1, 0 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_INT_ARRAY,       1, 0 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_LONG_ARRAY,      1, 0 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_FLOAT_ARRAY,     1, 0 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_DOUBLE_ARRAY,    1, 0 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_STRING,          1, 0 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_BYTE_ARRAY_4,    0, 4 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_BYTE_ARRAY_8,    0, 8 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_BYTE_ARRAY_16,   0, 16 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_BYTE_ARRAY_20,   0, 20 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_BYTE_ARRAY_32,   0, 32 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_BYTE_ARRAY_64,   0, 64 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_BYTE_ARRAY_128,  0, 128 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_BYTE_ARRAY_256,  0, 256 ) );
    FudgeTypeDictionary_addType ( dictionary, FudgeFieldType_create ( FUDGE_TYPE_BYTE_ARRAY_512,  0, 512 ) );
    
    return dictionary;
}

void FudgeTypeDictionary_destroy ( FudgeTypeDictionaryRef dictionary )
{
    FudgeTypeNode_destroy ( dictionary->nodes );
    free ( dictionary );
}

int FudgeTypeDictionary_addType ( FudgeTypeDictionaryRef dictionary, FudgeFieldType * type )
{
    FudgeTypeNode * node,
                  * * next;

    if ( ! dictionary ) return -1;  /* TODO Set errno */
    if ( ! type ) return -1; /* TODO Set errno */

    /* First node in dictionary: simply set the node point */
    if ( ! ( node = dictionary->nodes ) )
        return ( dictionary->nodes = FudgeTypeNode_create ( type ) ) ? 0 : -1;

    /* Not the first node: search down the tree until a matching typeId is found of a leaf node is hit */
    for ( ; ; )
    {
        if ( node->type->typeId == type->typeId )
        {
            /* Node matches new type: replace the existing contents */
            free ( node->type );
            node->type= type;
            return 0;
        }
     
        if ( ! *( next = node->type->typeId < type->typeId ? &( node->left ) : &( node->right ) ) )
        {
            /* Leaf node encountered: add new node (with new type) to current branch */
            return ( *next = FudgeTypeNode_create ( type ) ) ? 0 : -1;
        }

        node = *next;
    }

    return 0;
}

int FudgeTypeDictionary_dumpToFile ( FudgeTypeDictionaryRef dictionary, FILE * file )
{
    int error;

    if ( ! dictionary ) return -1;    /* TODO Set errno */
    if ( ! file ) return -1;          /* TODO Set errno */

    fprintf ( file, "FudgeTypeDictionary:\n" );
    if ( ( error = FudgeTypeNode_dumpToFile ( dictionary->nodes, file ) ) )
        return error;
    return 0;
}

