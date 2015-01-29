#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# As we have not included any thing still in the package but the 
# autoconf required configure.ac to do things and thus this file
# we are creating

# From Reading Manual I come to know that it scan all the files
# of directories and sub-directories and include the various test
# configuration as we include in our program.

# Minimum Autoconf running version number required. Below the 
# mentioned version creates error thus define it.
AC_PREREQ([2.63])

# Package Name and other things
AC_INIT([PDFTextExtraction], [0.1], [vineetgupta22@gmail.com])

#for checking cross compiling
AC_CANONICAL_TARGET
AC_CANONICAL_HOST


#enabling automake
AM_INIT_AUTOMAKE([1.11 foreign])


# Configuration of Source Directory
AC_CONFIG_SRCDIR([src/Main.c])


# Creating global config.h
AC_CONFIG_HEADERS([config.h])


# Library tools Macro Directory
AC_CONFIG_MACRO_DIR([m4])


# Checks for programs.
AC_LANG([C])
AC_PROG_CC_C99


#In Case Compiliting through cygwin for windows
LT_INIT([win32-dll shared pic-only])


#in case compiling on linux
AC_MSG_CHECKING([if $target_os is Linux])
if expr "$target_os" : "[[Ll]]inux.*" > /dev/null
then
	TARGET_LINUX="true"
	AC_MSG_RESULT([yes])
	AC_DEFINE([LAW_OS_LINUX], [1], [Whether we build for Linux])
else
	TARGET_LINUX="false"
	AC_MSG_RESULT([no])
fi
AC_SUBST(TARGET_LINUX)

#checking host as cygwin or not
AC_MSG_CHECKING([for build host cygwin])
case "$host_os" in
	*mingw* | *cygwin*)
		enable_libtool_shared=yes
		AC_DEFINE([LAW_CYGWIN], [], [Define if using cygwin])
		;;
	*)
		enable_libtool_shared=no
		;;
esac
AC_MSG_RESULT([$enable_libtool_shared])


#in case windows compiling enable libtool shared library flags
AC_MSG_CHECKING([whether shared library is needed to compile])
if test "$enable_libtool_shared" = yes; then
        LDFLAGS="$LDFLAGS -shared $lt_prog_compiler_pic -Wl,--kill-at"
        libtool_shared_enable=yes
else
        libtool_shared_enable=no
fi

AC_MSG_RESULT($libtool_shared_enable)


#checking windows icon creation tool
AC_CHECK_TOOL([WINDRES], [windres], [no])
#checking windows icon creation tool
AM_CONDITIONAL([PDFWINDRES], [test "$WINDRES" != "no"])

# Checks for header files.
AC_CHECK_HEADERS([stdio.h string.h stdlib.h unistd.h dirent.h sys/types.h sys/stat.h fcntl.h])

# Creating of Make file according to the sub-directories provided and
# File Makefile.am is available
AC_CONFIG_FILES([Makefile
                 inc/Makefile
                 src/Makefile
				 ])
AC_OUTPUT
	
AC_MSG_RESULT([---------------------
Current Configuration
---------------------
LawInfo is configured as follows. Please verify that this configuration matches your expectations.

Current Configuration Host system type: $host
Current Build system type: $build
Current Target system type: $target

If everything is correct, you can now run 'make' and (optionally)
Otherwise, you can run './configure' again
-------------------------------------------------------------------------
])

