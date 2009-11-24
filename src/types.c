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
#include "fudge/types.h"

fudge_bool FudgeType_typeIsFixedWidth ( fudge_type_id type )
{
    return FudgeType_getFixedWidth ( type ) != -1;
}

fudge_i32 FudgeType_getFixedWidth ( fudge_type_id type )
{
    switch ( type )
    {
        case FUDGE_TYPE_INDICATOR:  return 0;
        case FUDGE_TYPE_BOOLEAN:    return 1;
        case FUDGE_TYPE_BYTE:       return 1;
        case FUDGE_TYPE_SHORT:      return 2;
        case FUDGE_TYPE_INT:        return 4;
        case FUDGE_TYPE_LONG:       return 8;
        case FUDGE_TYPE_FLOAT:      return 4;
        case FUDGE_TYPE_DOUBLE:     return 8;

        case FUDGE_TYPE_BYTE_ARRAY_4:   return 4;
        case FUDGE_TYPE_BYTE_ARRAY_8:   return 8;
        case FUDGE_TYPE_BYTE_ARRAY_16:  return 16;
        case FUDGE_TYPE_BYTE_ARRAY_20:  return 20;
        case FUDGE_TYPE_BYTE_ARRAY_32:  return 32;
        case FUDGE_TYPE_BYTE_ARRAY_64:  return 64;
        case FUDGE_TYPE_BYTE_ARRAY_128: return 128;
        case FUDGE_TYPE_BYTE_ARRAY_256: return 256;
        case FUDGE_TYPE_BYTE_ARRAY_512: return 512;

        default:
            return -1;
    }
}

