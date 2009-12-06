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
#ifndef INC_FUDGE_REGISTRY_H
#define INC_FUDGE_REGISTRY_H

#include "fudge/status.h"
#include "fudge/types.h"

#ifdef __cplusplus
    extern "C" {
#endif

#define FUDGE_REGISTRY_SIZE 256

/* Prototype for bytes-to-FudgeField decoder function. */
typedef FudgeStatus ( *FudgeTypeDecoder ) ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data );

/* Prototype for FudgeField-to-bytes encoder function. The implementation
   must increment *data to one-byte past the end of the newly encoded field
   (this avoids the need for calling code to know the width of each field. */
typedef FudgeStatus ( *FudgeTypeEncoder ) ( const FudgeField * field, fudge_byte * * data );

typedef enum
{
    FUDGE_TYPE_PAYLOAD_LOCAL    = 0x0,  /* Data is stored in a local type */
    FUDGE_TYPE_PAYLOAD_BYTES    = 0x1,  /* Data is stored as an array of bytes */
    FUDGE_TYPE_PAYLOAD_SUBMSG   = 0x2   /* Data is stored as a FudgeMsg instance */
} FudgeTypePayload;

typedef struct
{
    fudge_type_id type;         /* The Id of the type */
    fudge_i32 fixedwidth;       /* The width of the type if fixed, otherwise -1 */
    FudgeTypePayload payload;   /* How the type's data is stored */

    /* Function pointers */
    FudgeTypeDecoder decoder;
    FudgeTypeEncoder encoder;
    /* TODO Type coercer */
} FudgeTypeDesc;

/* Initialises the type registry and populates it with the default types
   (see types.h). */
extern FudgeStatus FudgeRegistry_init ( );

extern const FudgeTypeDesc * FudgeRegistry_getTypeDesc ( fudge_type_id type );

#ifdef __cplusplus
    }
#endif

#endif

