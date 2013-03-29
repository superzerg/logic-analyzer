#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(logic-analyzer-rpi, 0.1)
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
AC_CHECK_LIB([bcm2835],[bcm2835_init])
AC_CHECK_LIB([mgl],[mgl_create_data])
AC_CHECK_LIB([rt],[clock_gettime])

# Checks for header files.
#AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([clock_gettime])
AC_CONFIG_FILES([Makefile src/Makefile doc/Makefile])
AC_OUTPUT()
