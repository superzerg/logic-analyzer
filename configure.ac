#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

#TODO: use variables HAVE_<HEADER>_H in headers or at least inturupt configure if not found

AC_PREREQ([2.69])
AC_INIT(logic-analyzer-rpi,0.1.1)
AM_INIT_AUTOMAKE()

#AC_CONFIG_SRCDIR([logic_analyzer.cpp])

# Checks for programs.
AC_CHECK_PROGS([DOXYGEN],[doxygen])
if test -z "$DOXYGEN";
    then AC_MSG_WARN([Doxygen not found - Continue without Doxygen support])
fi
AM_CONDITIONAL([HAVE_DOXYGEN],[test -n "$DOXYGEN"])
AM_COND_IF([HAVE_DOXYGEN],[AC_CONFIG_FILES([doc/Doxyfile])])
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
AC_SEARCH_LIBS([bcm2835_init],[bcm2835])
AC_SEARCH_LIBS([mgl_create_data],[mgl])
AC_SEARCH_LIBS([clock_gettime],[rt])

# Checks for header files.
AC_CHECK_HEADERS([bcm2835.h])
AC_CHECK_HEADERS([stdio.h])
AC_CHECK_HEADERS([time.h])
#next one does not work ... why ??
#AC_CHECK_HEADERS([mgl/mgl_zb.h])
AC_CHECK_HEADERS([stdarg.h])
AC_CHECK_HEADERS([getopt.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([clock_gettime])
AC_CONFIG_FILES([Makefile src/Makefile doc/Makefile])
AC_OUTPUT()
