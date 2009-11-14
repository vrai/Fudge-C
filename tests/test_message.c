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
#include "fudge/message.h"
#include "simpletest.h"

DEFINE_TEST( FieldFunctions )
    FudgeMsg message, submessage;

    /* Construct an empty message */
    TEST_EQUALS_INT( FudgeMsg_create ( &message ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 0 );

    /* Add indicator, boolean, integer and float fields */
    TEST_EQUALS_INT( FudgeMsg_addFieldIndicator ( message, 0 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldBool ( message, 0, FUDGE_TRUE ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 2 );

    /* TODO Add fixed width byte arrays in a sub message */

    /* TODO Add empty and populated strings */

    /* TODO Add empty and populated arrays in a sub message */

    /* Add an empty sub message */
    TEST_EQUALS_INT( FudgeMsg_create ( &submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_addFieldMsg ( message, "Empty SubMessage", submessage ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeMsg_release ( submessage ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeMsg_numFields ( message ), 3 );

    /* Clean up */
    TEST_EQUALS_INT( FudgeMsg_release ( message ), FUDGE_OK );
END_TEST

DEFINE_TEST_SUITE( Message )
    REGISTER_TEST( FieldFunctions )
END_TEST_SUITE

