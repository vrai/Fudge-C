dnl @synopsis FM_TEST_SIZEOF( TYPE, EXPECTED_SIZE )
dnl
dnl Retrieves the size (in bytes) of a type and compares it against the expected size.
dnl If the sizes don't match a failure is generated.
dnl
dnl Example:
dnl
dnl     TE_TEST_SIZEOF(float, 4)
dnl     TE_TEST_SIZEOF(double, 8)
dnl
dnl @category Misc
dnl @author Vra Stacey <vrai.stacey@gmail.com>
dnl @version 2009-11-12
dnl @license AllPermissive
AC_DEFUN([FM_TEST_SIZEOF],dnl
[dnl
AC_CHECK_SIZEOF($1)dnl
AS_VAR_PUSHDEF([_SIZE],[ac_cv_sizeof_$1])dnl
AS_IF([ test $_SIZE -ne $2],dnl
    AC_MSG_FAILURE([$1 has width $_SIZE not $2]))dnl
AS_VAR_POPDEF([_SIZE])dnl
])
