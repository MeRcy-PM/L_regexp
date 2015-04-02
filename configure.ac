#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(toy_grep, 1.0, 240560351@qq.com)
AM_INIT_AUTOMAKE(toy_grep, 1.0)
AC_CONFIG_SRCDIR([matcher.hpp])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
AC_ARG_ENABLE(debug-mode,
     AS_HELP_STRING(--disable-debug-mode, disable support for running in debug mode),
	 [case "$enableval" in
		yes) enable_debug_mode=yes;;
		no) enable_debug_mode=no;;
		*) AC_MSG_ERROR([bad value $enableval for --enable-debug-mode]);;
	  esac],
	 [enable_debug_mode=no])
AM_CONDITIONAL(BUILD_DEBUG_MODE, test x$enable_debug_mode = xyes)
# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE

# Checks for library functions.
AC_FUNC_STAT
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
