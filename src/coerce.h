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
#ifndef INC_COERCE_ENCODE_H
#define INC_COERCE_ENCODE_H

#include "fudge/status.h"
#include "fudge/types.h"

FudgeStatus FudgeType_coerceDefault   ( const FudgeField * source, const fudge_type_id type, FudgeFieldData * target, fudge_i32 * numbytes );

FudgeStatus FudgeType_coerceBoolean   ( const FudgeField * source, const fudge_type_id type, FudgeFieldData * target, fudge_i32 * numbytes );
FudgeStatus FudgeType_coerceByte      ( const FudgeField * source, const fudge_type_id type, FudgeFieldData * target, fudge_i32 * numbytes );
FudgeStatus FudgeType_coerceShort     ( const FudgeField * source, const fudge_type_id type, FudgeFieldData * target, fudge_i32 * numbytes );
FudgeStatus FudgeType_coerceInt       ( const FudgeField * source, const fudge_type_id type, FudgeFieldData * target, fudge_i32 * numbytes );
FudgeStatus FudgeType_coerceLong      ( const FudgeField * source, const fudge_type_id type, FudgeFieldData * target, fudge_i32 * numbytes );
FudgeStatus FudgeType_coerceFloat     ( const FudgeField * source, const fudge_type_id type, FudgeFieldData * target, fudge_i32 * numbytes );
FudgeStatus FudgeType_coerceDouble    ( const FudgeField * source, const fudge_type_id type, FudgeFieldData * target, fudge_i32 * numbytes );

#endif

