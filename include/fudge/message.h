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
#ifndef INC_FUDGE_MESSAGE_H
#define INC_FUDGE_MESSAGE_H

#include "fudge/message_ex.h"

#ifdef __cplusplus
    extern "C" {
#endif

/* Note on thread safety:
 
   The Fudge-C API is thread-safe but the data structures are not reentrant.
   Multiple threads may use the API but any given object (a FudgeMsg instance
   for example) must only be used by one thread at a time.

   Read-only exception to the above rule:

   It is actually safe to use FudgeMsg instances (and their associated fields)
   across multiple threads if access is strictly READ-ONLY in nature. However
   the Fudge-C encoding operations are NOT read-only (they use per-message
   storage for efficiency) and as such should not be used concurrently on any
   single FudgeMsg instance. */

/* FudgeMsg objects are reference counted and should only be created or freed
   using the API provided. On creation (using FudgeMsg_create or as the output
   of a decode operation) the reference count will be one. If the count falls
   to zero the message and its fields will be destroyed.
   Sub messages (message fields) have their reference count increased by one
   when added to another message and decreased by one if the parent message is
   destroyed. */
FUDGEAPI FudgeStatus FudgeMsg_create ( FudgeMsg * messageptr );
FUDGEAPI FudgeStatus FudgeMsg_retain ( FudgeMsg message );
FUDGEAPI FudgeStatus FudgeMsg_release ( FudgeMsg message );

/* Returns the number of fields within the message, zero if the message is
   a NULL pointer. */
FUDGEAPI unsigned long FudgeMsg_numFields ( FudgeMsg message );

/* The add field functions are all similar. The messagem, value (if
   applicable) and value size/number if elements (for variable size types) are
   mandatory. The name is optional and can either be a zero terminated char
   array or a NULL pointer. The ordinal is also optional and should be a
   pointer to the ordinal value or NULL. */
FUDGEAPI FudgeStatus FudgeMsg_addFieldIndicator ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal );

FUDGEAPI FudgeStatus FudgeMsg_addFieldBool ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, fudge_bool value );
FUDGEAPI FudgeStatus FudgeMsg_addFieldByte ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, fudge_byte value );
FUDGEAPI FudgeStatus FudgeMsg_addFieldI16  ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, fudge_i16 value );
FUDGEAPI FudgeStatus FudgeMsg_addFieldI32  ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, fudge_i32 value );
FUDGEAPI FudgeStatus FudgeMsg_addFieldI64  ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, fudge_i64 value );
FUDGEAPI FudgeStatus FudgeMsg_addFieldF32  ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, fudge_f32 value );
FUDGEAPI FudgeStatus FudgeMsg_addFieldF64  ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, fudge_f64 value );

/* As stated above, adding the message will increase its reference count. */
FUDGEAPI FudgeStatus FudgeMsg_addFieldMsg ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, FudgeMsg value );

FUDGEAPI FudgeStatus FudgeMsg_addFieldByteArray ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, const fudge_byte * bytes,  fudge_i32 numbytes );
FUDGEAPI FudgeStatus FudgeMsg_addFieldI16Array  ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, const fudge_i16 * ints,    fudge_i32 numints );
FUDGEAPI FudgeStatus FudgeMsg_addFieldI32Array  ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, const fudge_i32 * ints,    fudge_i32 numints );
FUDGEAPI FudgeStatus FudgeMsg_addFieldI64Array  ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, const fudge_i64 * ints,    fudge_i32 numints );
FUDGEAPI FudgeStatus FudgeMsg_addFieldF32Array  ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, const fudge_f32 * floats,  fudge_i32 numfloats );
FUDGEAPI FudgeStatus FudgeMsg_addFieldF64Array  ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, const fudge_f64 * doubles, fudge_i32 numdoubles );

FUDGEAPI FudgeStatus FudgeMsg_addFieldString ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, FudgeString string );

FUDGEAPI FudgeStatus FudgeMsg_addField4ByteArray   ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, const fudge_byte * bytes );
FUDGEAPI FudgeStatus FudgeMsg_addField8ByteArray   ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, const fudge_byte * bytes );
FUDGEAPI FudgeStatus FudgeMsg_addField16ByteArray  ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, const fudge_byte * bytes );
FUDGEAPI FudgeStatus FudgeMsg_addField20ByteArray  ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, const fudge_byte * bytes );
FUDGEAPI FudgeStatus FudgeMsg_addField32ByteArray  ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, const fudge_byte * bytes );
FUDGEAPI FudgeStatus FudgeMsg_addField64ByteArray  ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, const fudge_byte * bytes );
FUDGEAPI FudgeStatus FudgeMsg_addField128ByteArray ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, const fudge_byte * bytes );
FUDGEAPI FudgeStatus FudgeMsg_addField256ByteArray ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, const fudge_byte * bytes );
FUDGEAPI FudgeStatus FudgeMsg_addField512ByteArray ( FudgeMsg message, const FudgeString name, const fudge_i16 * ordinal, const fudge_byte * bytes );

/* Used to add arbitary types to a message. The data is stored and encoded as
   a simple array of bytes. No endian conversion will be applied to the
   contents. */
FUDGEAPI FudgeStatus FudgeMsg_addFieldOpaque ( FudgeMsg message,
                                             fudge_type_id type,
                                             const FudgeString name,
                                             const fudge_i16 * ordinal,
                                             const fudge_byte * bytes,
                                             fudge_i32 numbytes );

/* Creates a FudgeMsgField and adds it to the message. The name and ordinal
   are optional and can be NULL, other values must be valid.
   Note that for byte data (strings, arrays, etc), the memory is not copied.
   It is the job of the calling code to ensure that this memory is not
   destroyed. The FudgeMsg will destroy the memory when it is released.
   The same applies to submessage fields: the message's reference count will
   not be increased, but it will be decreased when the parent is destroyed. */
FUDGEAPI FudgeStatus FudgeMsg_addFieldData ( FudgeMsg message,
                                           fudge_type_id type,
                                           const FudgeString name,
                                           const fudge_i16 * ordinal,
                                           FudgeFieldData * data,
                                           fudge_i32 numbytes );

/* Retrieves the field at the index provided (returns FUDGE_INVALID_INDEX if
   the index is out of range). Note that the field's contents are not copied
   and remained owned by the parent message (do not release the message until
   the fields contents are no longer required).
   Linear time operation. */
FUDGEAPI FudgeStatus FudgeMsg_getFieldAtIndex ( FudgeField * field, FudgeMsg message, unsigned long index );

/* As above, but retrieves the first field with the name provided (which must
   be a valid, null terminated char array). Returns FUDGE_INVALID_NAME if no
   field in the message has the name.
   Linear time operation. */
FUDGEAPI FudgeStatus FudgeMsg_getFieldByName ( FudgeField * field, FudgeMsg message, const FudgeString name );

/* As above, but retrieves the first field with the ordinal provided. Returns
   FUDGE_INVALID_ORDINAL if not field in the message has the ordinal.
   Linear time operation. */
FUDGEAPI FudgeStatus FudgeMsg_getFieldByOrdinal ( FudgeField * field, FudgeMsg message, fudge_i16 ordinal );

/* Bulk field retrieval. Retrieves the fields in index order up to the end of
   the message or until "numfields" is reached. Returns the number of fields
   retrieved, -1 if the message or field pointer is NULL. The field array
   provided must be large enough to hold "numfields" fields. As with other
   field retrieval functions, the field contents remain owned by the message.
   Linear time operation. */
FUDGEAPI fudge_i32 FudgeMsg_getFields ( FudgeField * fields, fudge_i32 numfields, const FudgeMsg message );

#ifdef __cplusplus
    }
#endif

#endif

