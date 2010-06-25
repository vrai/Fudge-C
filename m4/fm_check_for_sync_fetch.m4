dnl @synopsis FM_CHECK_FOR_SYNC_FETCH( )
dnl
dnl Checks that the target can compile a program using __sync_fetch_and_add
dnl and __sync_sub_and_fetch; the GCC atomic functions.
dnl
dnl If the target does provided the functionality fm_cv_check_for_sync_fetch
dnl is set to "yes", otherwise it's set to "no".
dnl
dnl @category Misc
dnl @author Vrai Stacey <vrai.stacey@gmail.com>
dnl @version 2010-06-25
dnl @license AllPermissive
AC_DEFUN([FM_CHECK_FOR_SYNC_FETCH],dnl
[dnl
AC_CACHE_CHECK([for __sync_fetch_and_add], [fm_cv_check_for_sync_fetch],dnl
    [dnl
        AC_TRY_LINK([],[int value = 0, low, high;
                        low = __sync_fetch_and_add ( &value, 1 );
                        high = __sync_sub_and_fetch ( &value, 1 );
                        return 0;],dnl
            [fm_cv_check_for_sync_fetch='yes'],dnl
            [fm_cv_check_for_sync_fetch='no'])dnl
    ])dnl
])
