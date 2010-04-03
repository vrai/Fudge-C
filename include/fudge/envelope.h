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

