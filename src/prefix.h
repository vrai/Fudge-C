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
#ifndef INC_FUDGE_PREFIX_H
#define INC_FUDGE_PREFIX_H

#include "fudge/status.h"
#include "fudge/types.h"

typedef struct
{
    fudge_bool fixedwidth;      /* Field has a fixed width determined by its type */
    fudge_byte variablewidth;   /* Field has a variable width encoded in the field header */
    fudge_bool ordinal;         /* Field has a ordinal, application specific */
    fudge_bool name;            /* Field has a name */
} FudgeFieldPrefix;

extern FudgeStatus FudgePrefix_decodeFieldPrefix ( FudgeFieldPrefix * prefix, fudge_byte byte );

extern fudge_byte FudgePrefix_encodeFieldPrefix ( const FudgeFieldPrefix prefix );

#endif
