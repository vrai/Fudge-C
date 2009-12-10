# Copyright (C) 2009 - 2009, Vrai Stacey.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
#     
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
 
### Need modern autoconf, only 2.60+ has the AC_TYPE_INT*_T macros
AC_PREREQ([2.60])

AC_INIT([Fudge-C], [0.1], [vrai.stacey@gmail.com])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADER(include/config.h)
AC_CONFIG_FILES([Makefile \
                 src/Makefile \
                 include/Makefile \
                 include/fudge/Makefile
                 tests/Makefile])
AM_INIT_AUTOMAKE(fudge-c,0.1)

### Make sure we're in the right directory
AC_CONFIG_SRCDIR(./src/message.c)

### Make sure that the necessary programs are available
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL

### Make sure the inline keyword is present
AC_C_INLINE

### Check for fixed width integer types
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SIZE_T

### Make sure the floating point types are the right size
FM_TEST_SIZEOF(float,4)
FM_TEST_SIZEOF(double,8)

### Check for the presence of key header files
AC_CHECK_HEADERS_ONCE(arpa/inet.h)
AC_CHECK_HEADERS_ONCE(float.h)
AC_CHECK_HEADERS_ONCE(math.h)
AC_CHECK_HEADERS_ONCE(setjmp.h)
AC_CHECK_HEADERS_ONCE(stdarg.h)
AC_CHECK_HEADERS_ONCE(time.h)

### Check for the presence of key functions missing (or renamed) in some compilers
AC_CHECK_FUNC(dup, AC_DEFINE(HAS_DUP, 1, [Define to 1 if dup is available.]))
AC_CHECK_FUNC(getpid, AC_DEFINE(HAS_GETPID, 1, [Define to 1 if getpid is available.]))
AC_CHECK_FUNC(gmtime_r, AC_DEFINE(HAS_GMTIME_R, 1, [Define to 1 if gmtime_r is available.]))
AC_CHECK_FUNC(_gmtime_s, AC_DEFINE(HAS__GMTIME_S, 1, [Define to 1 if _gmtime_s is available.]))
AC_CHECK_FUNC(snprintf, AC_DEFINE(HAS_SPRINTF, 1, [Define to 1 if snprintf is available.]))
AC_CHECK_FUNC(sprintf_s, AC_DEFINE(HAS_SPRINTF_S, 1, [Define to 1 if sprintf_s is available.]))
AC_CHECK_FUNC(strdup, AC_DEFINE(HAS_STRDUP, 1, [Define to 1 if strdup is available.]))
AC_CHECK_FUNC(_strdup, AC_DEFINE(HAS__STRDUP, 1, [Define to 1 if _strdup is available.]))

### Crank the warnings up to maximum
AX_CFLAGS_WARN_ALL

AX_CHECK_COMPILER_FLAGS(-fno-strict-aliasing,CFLAGS+=" -fno-strict-aliasing")

### The config.h file will be includes in the final package - make sure its
### definitions don't clash with anything else
AX_PREFIX_CONFIG_H([include/fudge/config.h], FUDGE, [include/config.h])

AC_OUTPUT
