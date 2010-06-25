/**
 * Copyright (C) 2009 - 2010; Open Gamma, Vrai Stacey.
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
#ifndef INC_FUDGE_ATOMIC_H
#define INC_FUDGE_ATOMIC_H

#if defined(_MT) && defined(FUDGE_HAVE_INTRIN_H)
    // Microsoft compilers, use INTRIN.H functions in MultiThread builds
#   include <intrin.h>
#   define AtomicIncrementAndReturn(var) _InterlockedIncrement(&var)
#   define AtomicDecrementAndReturn(var) _InterlockedDecrement(&var)
#   define AtomicExchangePointer(var,val) (void*)_InterlockedExchange((long*)&var,(long)val)
#elif defined(_MT) && defined(FUDGE_HAS_SYNC_FETCH_AND_ADD)
    // GCC 4.1+ atomic functions
#   define AtomicIncrementAndReturn(var) __sync_add_and_fetch ( &var, 1 )
#   define AtomicDecrementAndReturn(var) __sync_sub_and_fetch ( &var, 1 )
#   define AtomicExchangePointer(var, val) __sync_lock_test_and_set ( &var, val )
#else
    // No multi-threading support - just use standard operations
    // TODO AtomicExchangePointer generates errors on GCC due to size mismatch
#   define AtomicIncrementAndReturn(var) (++var)
#   define AtomicDecrementAndReturn(var) (--var)
#   define AtomicExchangePointer(var,val) ((var=val)&&0)
#endif

#endif /* ifndef INC_FUDGE_ATOMIC_H */
