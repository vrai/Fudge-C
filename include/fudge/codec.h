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
#ifndef INC_FUDGE_CODEC_H
#define INC_FUDGE_CODEC_H

#include "fudge/message.h"

#ifdef __cplusplus
    extern "C" {
#endif

/* Note on thread safety:
 
   The Fudge-C API is thread-safe and the codec functions can be used
   concurrently by multiple threads. However, as explained in message.h, the
   Fudge-C data structures (such as FudgeMsg) are NOT reentrant and a FudgeMsg
   instance must not be encoded or updated by more than one thread at any
   given time. */

/* Decodes the fudge encoded message within the byte array provided and places
   the results in a FudgeMsg instance. The "message" field of the
   FudgeMsgEnvelope provided is set to this new message instance. The message
   is decoded upfront, so any errors will be detected immediately. */
FUDGEAPI FudgeStatus FudgeCodec_decodeMsg ( FudgeMsgEnvelope * envelope, const fudge_byte * bytes, fudge_i32 numbytes );

/* Encodes the envelope provided (which must contain a valid FudgeMsg
   instance) in to a newly allocated block of memory. The bytes pointer is set
   to this new block and numbytes is set to its size. It is the responsibilty
   of the calling code to free the block when no longer needed. */
FUDGEAPI FudgeStatus FudgeCodec_encodeMsg ( FudgeMsgEnvelope envelope, fudge_byte * * bytes, fudge_i32 * numbytes );

#ifdef __cplusplus
    }
#endif

#endif

