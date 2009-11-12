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

### Check for the presence of strdup (missing in C99 compatible compilers)
AC_CHECK_FUNC(strdup, AC_DEFINE(HAS_STRDUP, 1, [Define to 1 if strdup is available.]))

### Crank the warnings up to maximum
AX_CFLAGS_WARN_ALL

### The config.h file will be includes in the final package - make sure its
### definitions don't clash with anything else
AX_PREFIX_CONFIG_H([include/fudge/config.h], FUDGE, [include/config.h])

AC_OUTPUT
