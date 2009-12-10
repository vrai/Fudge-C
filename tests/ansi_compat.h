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

#ifndef INC_FUDGE_ANSI_COMPAT_H
#define INC_FUDGE_ANSI_COMPAT_H

/**
 * Function compatibility for not-quite ANSI C library support. Mostly used
 * for Microsoft Visual C builds on Windows. This isn't particularly
 * polished at the moment, and has only been tested on Visual Studio 2008.
 */

#include "fudge/config.h"

#if !defined(FUDGE_HAS_GMTIME_R) && !defined(gmtime_r)
#ifdef FUDGE_HAS__GMTIME64_S
#define gmtime_r(__timer,__tp) _gmtime64_s(__tp,__timer)
#endif /* ifndef FUDGE_HAS_GMTIME64_S */
#ifndef gmtime_r
#error "Substitute for required gmtime_r not available on this platform"
#endif /* ifndef gmtime_r */
#endif /* ifndef FUDGE_HAS_CMTIME_R */

#endif /* ifndef INC_FUDGE_ANSI_COMPAT_H */
