### Need modern autoconf, only 2.60+ has the AC_TYPE_INT*_T macros
AC_PREREQ([2.60])

AC_INIT([Fudge-C], [0.1], [vrai.stacey@gmail.com])
AC_CONFIG_AUX_DIR(config)
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
AC_PROG_RANLIB

### Make sure the inline keyword is present
AC_C_INLINE

### Check for fixed width integer types
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SIZE_T

### Make sure the floating point types are the right size
AC_CHECK_SIZEOF(float)
AC_CHECK_SIZEOF(double)

### TODO - Replace these with two macro calls?
if test $ac_cv_sizeof_float -ne 4; then
    AC_MSG_ERROR(Float is not 4 bytes wide! Reports width of $ac_cv_sizeof_float)
fi
if test $ac_cv_sizeof_double -ne 8; then
    AC_MSG_ERROR(Double is not 8 bytes wide! Reports width of $ac_cv_sizeof_double)
fi

### Check for the presence of strdup (missing in C99 compatible compilers)
AC_CHECK_FUNC(strdup, AC_DEFINE(HAS_STRDUP, 1, [Define to 1 if strdup is available.]))

### The config.h file will be includes in the final package - make sure it
### doesn't clash with anything else
AX_PREFIX_CONFIG_H([include/fudge/config.h], FUDGE, [include/config.h])

AC_OUTPUT
