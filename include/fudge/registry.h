/**
 * Copyright (C) 2009 - 2010, Vrai Stacey.
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

/* Prototype for FudgeField type coercion. The function should populate the
   (non-null target and numbytes) depending on the target type. Any byte array
   types should have their contents copied, it is the job of calling code to
   release this memory. */
typedef FudgeStatus ( *FudgeTypeCoercer ) ( const FudgeField * source, const fudge_type_id type, FudgeFieldData * target, fudge_i32 * numbytes );

typedef enum
{
    FUDGE_TYPE_PAYLOAD_LOCAL    = 0x0,  /* Data is stored in a local type */
    FUDGE_TYPE_PAYLOAD_BYTES    = 0x1,  /* Data is stored as an array of bytes */
    FUDGE_TYPE_PAYLOAD_SUBMSG   = 0x2   /* Data is stored as a FudgeMsg instance */
} FudgeTypePayload;

/* Registers a user defined type in the registry. Returns FUDGE_OK if the type
   information is registered, FUDGE_NULL_POINTER if any of the function
   pointers are NULL, FUDGE_INVALID_USER_TYPE if the new type's payload is not
   one of FUDGE_TYPE_PAYLOAD_BYTES or FUDGE_TYPE_PAYLOAD_SUBMSG.
   
   Note that there is no thread protection around the registry;
   multiple reader threads is fine, but the calling code must ensure that one
   and only one thread is accessing the registry during a write operation such
   as this. */
FUDGEAPI FudgeStatus FudgeRegistry_registerType ( fudge_type_id type,
                                                  FudgeTypePayload payload,
                                                  FudgeTypeDecoder decoder,
                                                  FudgeTypeEncoder encoder,
                                                  FudgeTypeCoercer coercer );

#ifdef __cplusplus
    }
#endif

#endif

