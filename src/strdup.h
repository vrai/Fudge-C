/**
 * Copyright (C) 2009 - 2009, OpenGamma.
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

#ifndef INC_FUDGE_STRDUP_H
#define INC_FUDGE_STRDUP_H

#include "fudge/config.h"

#if !defined(FUDGE_HAS__STRDUP) && !defined(_strdup)

#ifdef FUDGE_HAS_STRDUP || defined(strdup)
#define _strdup strdup
#else /* ifdef FUDGE_HAS_STRDUP */
extern char *fudge_strdup (const char *string);
#define _strdup fudge_strdup
#define FUDGE_STRDUP_IMPL
#endif /* ifdef FUDGE_HAS_STRDUP */

#endif /* if !defined(FUDGE_HAS__STRDUP) && !defined(_strdup) */

#endif /* ifndef INC_FUDGE_STRDUP_H */
