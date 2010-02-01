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
#ifndef INC_FUDGE_REGISTRY_INTERNAL_H
#define INC_FUDGE_REGISTRY_INTERNAL_H

#include "fudge/registry.h"

typedef struct
{
    fudge_type_id type;         /* The Id of the type */
    fudge_i32 fixedwidth;       /* The width of the type if fixed, otherwise -1 */
    FudgeTypePayload payload;   /* How the type's data is stored */

    /* Function pointers */
    FudgeTypeDecoder decoder;
    FudgeTypeEncoder encoder;
    FudgeTypeCoercer coercer;
} FudgeTypeDesc;

/* Initialises the type registry and populates it with the default types
   (see types.h). Usually only called from fudge.c. */
FudgeStatus FudgeRegistry_init ( );

/* Retrieves a const pointer to the type description structure for a given
   type, or NULL if there is no type with the given Id. */
const FudgeTypeDesc * FudgeRegistry_getTypeDesc ( fudge_type_id type );

#endif

