#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(m53, 1.0, a.mari@gecoitalia.biz)
AC_CONFIG_SRCDIR([main.c])

PACKAGE=m53
VERSION=1.0.0

AM_INIT_AUTOMAKE($PACKAGE, $VERSION)


# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([wiringPi], [wiringPiSetup])
AC_CHECK_LIB(gd, gdImageCreate)

#CFLAGS="-lgd -lwiringPi -lpthread"
#LDFLAGS="-lgd -lwiringPi -lpthread"

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h sys/ioctl.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([gethostname memset])

AC_OUTPUT(Makefile)