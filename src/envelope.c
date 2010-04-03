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
#include "fudge/envelope.h"
#include "fudge/message.h"
#include "reference.h"

struct FudgeMsgEnvelopeImpl
{
    FudgeRefCount refcount;
    fudge_byte directives;
    fudge_byte schemaversion;
    fudge_i16 taxonomy;
    FudgeMsg message;
};

FudgeStatus FudgeMsgEnvelope_create ( FudgeMsgEnvelope * envelopeptr,
                                   fudge_byte directives,
                                   fudge_byte schemaversion,
                                   fudge_i16 taxonomy,
                                   FudgeMsg message )
{
    FudgeStatus status;

    if ( ! ( *envelopeptr = ( FudgeMsgEnvelope ) malloc ( sizeof ( struct FudgeMsgEnvelopeImpl ) ) ) )
        return FUDGE_OUT_OF_MEMORY;

    if ( ( status = FudgeRefCount_create ( &( ( *envelopeptr )->refcount ) ) ) != FUDGE_OK )
        goto release_and_fail;

    if ( ( status = FudgeMsg_retain ( message ) ) != FUDGE_OK )
        goto release_and_fail;

    ( *envelopeptr )->directives = directives;
    ( *envelopeptr )->schemaversion = schemaversion;
    ( *envelopeptr )->taxonomy = taxonomy;
    ( *envelopeptr )->message = message;

    return FUDGE_OK;

release_and_fail:
    free ( *envelopeptr );
    return status;
}

FudgeStatus FudgeMsgEnvelope_retain ( FudgeMsgEnvelope envelope )
{
    if ( ! envelope )
        return FUDGE_NULL_POINTER;

    FudgeRefCount_increment ( envelope->refcount );
    return FUDGE_OK;
}

FudgeStatus FudgeMsgEnvelope_release ( FudgeMsgEnvelope envelope )
{
    if ( ! envelope )
        return FUDGE_NULL_POINTER;

    if ( ! FudgeRefCount_decrementAndReturn ( envelope->refcount ) )
    {
        /* Last reference has been released - release the message and destroy the envelope */
        FudgeStatus status;

        if ( ( status = FudgeMsg_release ( envelope->message ) ) != FUDGE_OK )
            return status;

        if ( ( status = FudgeRefCount_destroy ( envelope->refcount ) ) != FUDGE_OK )
            return status;

        free ( envelope );
    }
    return FUDGE_OK;
}

fudge_byte FudgeMsgEnvelope_getDirectives ( const FudgeMsgEnvelope envelope )
{
    return envelope ? envelope->directives : 0;
}

fudge_byte FudgeMsgEnvelope_getSchemaVersion ( const FudgeMsgEnvelope envelope )
{
    return envelope ? envelope->schemaversion : 0;
}

fudge_i16 FudgeMsgEnvelope_getTaxonomy ( const FudgeMsgEnvelope envelope )
{
    return envelope ? envelope->taxonomy : 0;
}

FudgeMsg FudgeMsgEnvelope_getMessage ( const FudgeMsgEnvelope envelope )
{
    return envelope ? envelope->message : 0;
}

