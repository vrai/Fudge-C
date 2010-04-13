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
#ifndef INC_FUDGE_MSG_ENVELOPE_H
#define INC_FUDGE_MSG_ENVELOPE_H

#include "fudge/status.h"
#include "fudge/types.h"

#ifdef __cplusplus
    extern "C" {
#endif

/* The FudgeMsgEnvelope bundles the message payload (a FudgeMsg instance)
   together with message meta-information. These envelopes are used as the
   input and output for the Fudge codec's encoding/decoding operations.

   The current implementation has the following meta-data:

     - Processing directives:
         Unused in the current implementation.

     - Schema version:
         An application specific version number; how this is supported or
         handled is up to the calling code.

     - Taxonomy reference:
         An application specific taxonomy indicator; as with the schema
         version, this is for the calling code to support and handle.

    All envelopes must have a valid FudgeMsg instance. The message's reference
    count will be incremented on creation of the holding envelope and
    decremented when the envelope is destroyed. For example, to avoid leaking
    memory one must remember to decrement the FudgeMsg's reference count after
    using it create the envelope:

      FudgeMsg message;
      FudgeMsgEnvelope envelope;

      FudgeMsg_create ( &message );
      FudgeMsgEnvelope_create ( &envelope, 0, 0, 0, message );
      FudgeMsg_release ( message );

      ...

      FudgeMsgEnvelope_release ( envelope );

    For envelope instances returned by FudgeCodec_decodeMsg, the message
    contained within will have only one reference and so (assuming no
    additional references are retained) will be destroyed along with the
    envelope.

    As indicated by the above example, FudgeMsgEnvelope instances are
    reference counted and should only be created and/or freed using the API
    provided.

    FudgeMsgEnvelope follows the same thread-safety rules as the rest of the
    Fudge-C library. It safe to use in multi-threaded environment as long as
    no one instance of the type is used in more than one thread concurrently.
*/
FUDGEAPI FudgeStatus FudgeMsgEnvelope_create ( FudgeMsgEnvelope * envelopeptr,
                                            fudge_byte directives,
                                            fudge_byte schemaversion,
                                            fudge_i16 taxonomy,
                                            FudgeMsg message );
FUDGEAPI FudgeStatus FudgeMsgEnvelope_retain ( FudgeMsgEnvelope envelope );
FUDGEAPI FudgeStatus FudgeMsgEnvelope_release ( FudgeMsgEnvelope envelope );

/* Processing directives, for future use */
FUDGEAPI fudge_byte FudgeMsgEnvelope_getDirectives ( const FudgeMsgEnvelope envelope );

/* Application specific Schema version */
FUDGEAPI fudge_byte FudgeMsgEnvelope_getSchemaVersion ( const FudgeMsgEnvelope envelope );

/* Application specific taxonomy reference */
FUDGEAPI fudge_i16 FudgeMsgEnvelope_getTaxonomy ( const FudgeMsgEnvelope envelope );

/* The actual message */
FUDGEAPI FudgeMsg FudgeMsgEnvelope_getMessage ( const FudgeMsgEnvelope envelope );

#ifdef __cplusplus
    }
#endif

#endif

