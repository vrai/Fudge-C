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
#ifndef INC_FUDGE_ATOMIC_H
#define INC_FUDGE_ATOMIC_H

// TODO: Use the definitions from config.h to pull in what the system can
//       provide for atomic operations. As a last resort, fall back to
//       pthreads with locking. This is a quick hack to work with Visual
//       Studio builds on Windows.

#if defined(FUDGE_HAVE_INTRIN_H) && defined(_MT)
// Microsoft compilers, use INTRIN.H functions in MultiThread builds
#include <intrin.h>
#define AtomicIncrementAndReturn(var) _InterlockedIncrement(&var)
#define AtomicDecrementAndReturn(var) _InterlockedDecrement(&var)

#else
// Non atomic defaults
#define AtomicIncrementAndReturn(var) (++var)
#define AtomicDecrementAndReturn(var) (--var)
#endif

#endif /* ifndef INC_FUDGE_ATOMIC_H */
