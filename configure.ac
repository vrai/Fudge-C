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

AC_INIT([Fudge-C], [0.3], [vrai.stacey@gmail.com])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADER(include/config.h)
AC_CONFIG_FILES([Makefile \
                 src/Makefile \
                 include/Makefile \
                 include/fudge/Makefile
                 tests/Makefile])
AM_INIT_AUTOMAKE

### Define the API version, of the format CURRENT:REVISION:AGE. Changes to
### the version number should be applied as follows:
###
### API changed, backwards compatible:      +1 :  0 : +1
### API changed, not backwards compatible:  +1 :  0 :  0
### API unchanged, internals updated:        ? : +1 :  ?
API_VERSION="0:0:0"
AC_SUBST(API_VERSION)

### Make sure we're in the right directory
AC_CONFIG_SRCDIR(./src/message.c)

### Make sure that the necessary programs are available
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL

### Make sure the required C keywords are present
AC_C_CONST
AC_C_INLINE

### Check for fixed width integer types
AC_TYPE_INT8_T
AC_TYPE_UINT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_UINT32_T
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
AC_CHECK_FUNC(isnan, AC_DEFINE(HAS_ISNAN, 1, [Define to 1 if isnan is available.]))
AC_CHECK_FUNC(getpid, AC_DEFINE(HAS_GETPID, 1, [Define to 1 if getpid is available.]))
AC_CHECK_FUNC(gmtime_r, AC_DEFINE(HAS_GMTIME_R, 1, [Define to 1 if gmtime_r is available.]))
AC_CHECK_FUNC(_gmtime_s, AC_DEFINE(HAS__GMTIME_S, 1, [Define to 1 if _gmtime_s is available.]))
AC_CHECK_FUNC(snprintf, AC_DEFINE(HAS_SPRINTF, 1, [Define to 1 if snprintf is available.]))
AC_CHECK_FUNC(sprintf_s, AC_DEFINE(HAS_SPRINTF_S, 1, [Define to 1 if sprintf_s is available.]))

### Does the user want threading enabled?
AC_ARG_ENABLE(threads, [AS_HELP_STRING([--disable-threads], [disable thread safe reference counting])],
                       [with_threads='no'],
                       [with_threads='yes'])

if test "$with_threads" != 'no'
then
    # Threading is required, set the multi-threading flag and populate the
    # config variables necessary to pick a threading implementation.
    CFLAGS+=" -D_MT "

    # What potential multithreading constructs/libraries are available?
    AC_C_VOLATILE
    FM_CHECK_FOR_SYNC_FETCH()
    ACX_PTHREAD()

    # If an atomic integer implementation isn't available, fall back on using
    # pthreads (if available)
    if test "$fm_cv_check_for_sync_fetch" != 'no'
    then
        AC_MSG_NOTICE([will use __sync_fetch_and_* atomics to ensure thread-safe reference counting])
        AC_DEFINE(HAS_SYNC_FETCH_AND_ADD, 1, [Define to 1 if __sync_fetch_and_add is available])
    else
        AC_MSG_NOTICE([no atomic integer implementation found, falling back on pthreads])

        if test "$acx_pthread_ok" != 'no'
        then
            CFLAGS+="$PTHREAD_CFLAGS"
            LIBS="$PTHREAD_LIBS $LIBS"
            AC_DEFINE(HAS_PTHREADS, 1, [Define to 1 if pthreads are available.])
        else
            AC_MSG_ERROR([pthreads not found, cannot compiler thread-safe reference counting])
        fi
    fi
else
    AC_MSG_NOTICE([threading is disabled, reference counting will not be thread safe!])
fi

### Crank the warnings up to maximum
AX_CFLAGS_WARN_ALL

AX_CHECK_COMPILER_FLAGS(-fno-strict-aliasing,CFLAGS+=" -fno-strict-aliasing")

### The config.h file will be includes in the final package - make sure its
### definitions don't clash with anything else
AX_PREFIX_CONFIG_H([include/fudge/config.h], FUDGE, [include/config.h])

AC_OUTPUT
