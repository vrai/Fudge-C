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
#ifndef INC_FUDGE_HEADER_H
#define INC_FUDGE_HEADER_H

#include "fudge/status.h"
#include "fudge/types.h"

typedef struct
{
    fudge_byte directives;      /* Processing directives, reserved for future use */
    fudge_byte schemaversion;   /* Messaging schema version, application specific */
    fudge_i16 taxonomy;         /* Message taxonomy, application specific */
    fudge_i32 numbytes;         /* Message size in bytes */
} FudgeMsgHeader;

typedef struct
{
    fudge_type_id type;         /* Field type */
    fudge_byte widthofwidth;    /* Number of bytes used to hold field width, zero for fixed width */
    fudge_bool hasordinal;      /* If non-zero, then the ordinal field is populated */
    fudge_i16 ordinal;          /* Field ordinal, if hasordinal is non-zero  */
    const char * name;          /* Field name or NULL if no name */
} FudgeFieldHeader;

FudgeStatus FudgeHeader_decodeMsgHeader ( FudgeMsgHeader * header, const fudge_byte * bytes, fudge_i32 numbytes );

FudgeStatus FudgeHeader_decodeFieldHeader ( FudgeFieldHeader * header, fudge_i32 * consumed, const fudge_byte * bytes, fudge_i32 numbytes );
FudgeStatus FudgeHeader_encodeFieldHeader ( const FudgeFieldHeader * header, fudge_byte * * writepos );
FudgeStatus FudgeHeader_destroyFieldHeader ( FudgeFieldHeader header );

FudgeStatus FudgeHeader_getFieldWidth ( fudge_i32 * width, fudge_i32 * consumed, FudgeFieldHeader header, const fudge_byte * bytes, fudge_i32 numbytes );

const fudge_i16 * FudgeHeader_getOrdinal ( const FudgeFieldHeader * header );

#endif

