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

/* Selects the correct reference.c implementation, depending on whether
   multithreading support is required and the capabilities of the
   compiler/environment being used to build the library. */

#define _FUDGEREFCOUNTIMPL_DEFINED 1
#include "reference.h"
#include "fudge/config.h"

#if defined(_MT)
#   if defined(FUDGE_HAS_SYNC_FETCH_AND_ADD) || defined(FUDGE_HAVE_INTRIN_H)
#       include "reference_atomic.c"
#   elif defined(FUDGE_HAS_PTHREADS)
#       include "reference_pthreads.c"
#   else
#       error Cannot find a multithreaded implementation of reference.c that \
              supports the the current compiler/build environment.
#   endif
#else
#   include "reference_default.c"
#endif
