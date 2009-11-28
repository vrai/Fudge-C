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
#include "prefix.h"

FudgeStatus FudgePrefix_decodeFieldPrefix ( FudgeFieldPrefix * prefix, fudge_byte byte )
{
    prefix->fixedwidth = ( byte & 0x80 ) != 0;
    prefix->ordinal = ( byte & 0x10 ) != 0;
    prefix->name = ( byte & 0x08 ) != 0;

    prefix->variablewidth = ( byte & 0x60 ) >> 5;
    if ( prefix->variablewidth == 3 )
        prefix->variablewidth = 4;

    return FUDGE_OK;
}

fudge_byte FudgePrefix_encodeFieldPrefix ( const FudgeFieldPrefix prefix )
{
    fudge_byte encoded, varwidth;

    /* Only two bits are available for this, so 4 is stored as 3 */
    varwidth = prefix.variablewidth < 4 ? prefix.variablewidth : 3;

    encoded = ( prefix.fixedwidth ? 0 : varwidth ) << 5;
    encoded |= prefix.fixedwidth    ? 0x80 : 0;
    encoded |= prefix.ordinal       ? 0x10 : 0;
    encoded |= prefix.name          ? 0x08 : 0;

    return encoded;
}

