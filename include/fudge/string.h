/**
 * Copyright (C) 2010 - 2010, Vrai Stacey.
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
#ifndef INC_FUDGE_STRING_H
#define INC_FUDGE_STRING_H

#include "fudge/status.h"
#include "fudge/types.h"

#ifdef __cplusplus
    extern "C" {
#endif

FUDGEAPI FudgeStatus FudgeString_createFromASCII ( FudgeString * string, const char * chars, size_t numchars );
FUDGEAPI FudgeStatus FudgeString_createFromASCIIZ ( FudgeString * string, const char * chars );
FUDGEAPI FudgeStatus FudgeString_createFromUTF8 ( FudgeString * string, const fudge_byte * bytes, size_t numbytes );
FUDGEAPI FudgeStatus FudgeString_createFromUTF16 ( FudgeString * string, const fudge_byte * bytes, size_t numbytes );
FUDGEAPI FudgeStatus FudgeString_createFromUTF32 ( FudgeString * string, const fudge_byte * bytes, size_t numbytes );

FUDGEAPI FudgeStatus FudgeString_retain ( FudgeString string );
FUDGEAPI FudgeStatus FudgeString_release ( FudgeString string );

FUDGEAPI size_t FudgeString_getSize ( const FudgeString string );
FUDGEAPI const fudge_byte * FudgeString_getData ( const FudgeString string );

FUDGEAPI FudgeStatus FudgeString_convertToASCIIZ ( char * * target, const FudgeString string );
FUDGEAPI FudgeStatus FudgeString_convertToUTF16 ( fudge_byte * * target, size_t * numbytes, const FudgeString string );
FUDGEAPI FudgeStatus FudgeString_convertToUTF32 ( fudge_byte * * target, size_t * numbytes, const FudgeString string );

FUDGEAPI int FudgeString_compare ( const FudgeString left, const FudgeString right );

#ifdef __cplusplus
    }
#endif

#endif

