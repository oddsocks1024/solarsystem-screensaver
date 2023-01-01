#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])

# Package name and version
AC_INIT([solarsystem], [0.2.2], [packages@amiga-hardware.com])
AC_REVISION([$Revision: 0.2.2 $])

#Initialise Automake
AM_INIT_AUTOMAKE(-Wall -Werror dist-bzip2 dist-zip subdir-objects)

# Calculate the canonical host. Not used yet, but useful for debugging
AC_CANONICAL_HOST()

# We want a config.h
AC_CONFIG_HEADERS([config.h])

# Standard checks for various programs. (Note: C++ only needed by some tests)
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CXX
# This checks that a c++ is really found because AC_PROG_CXX with set g++ if it finds nothing
AC_DEFUN([rw_PROG_CXX_WORKS],
[AC_REQUIRE([AC_PROG_CXX])dnl
AC_CACHE_CHECK([whether the C++ compiler works],
               [rw_cv_prog_cxx_works],
               [AC_LANG_PUSH([C++])
                AC_LINK_IFELSE([AC_LANG_PROGRAM([], [])],
                               [rw_cv_prog_cxx_works=yes],
                               [rw_cv_prog_cxx_works=no])
                AC_LANG_POP([C++])])
])
rw_PROG_CXX_WORKS
AS_IF([test "$rw_cv_prog_cxx_works" = "no"],
      [AC_MSG_ERROR(Your C++ compiler does not work or is not installed. Please ensure at least g++ is available, [1])],
      [])
AC_PROG_MKDIR_P
AC_PROG_LN_S
AC_PROG_SED
AC_PROG_GREP

# Non standard checks for various programs
AC_PATH_PROGS([KDE_CONFIG], [kde4-config], [None], [$PATH:/usr/local/kde4/bin:/usr/kde4/bin])
PKG_PROG_PKG_CONFIG([])

# Check if we have gnome-screensaver support
PKG_CHECK_EXISTS([gnome-screensaver], [GNOMESAVER=yes], [GNOMESAVER=no])

AS_IF([test "$GNOMESAVER" = "yes"],
      [ GNOME_DESKTOP_DIR=`$PKG_CONFIG --variable=themesdir gnome-screensaver`
        GNOME_LNK_DIR=`$PKG_CONFIG --variable=privlibexecdir gnome-screensaver`
      ],
      [])
AM_CONDITIONAL([INSTALL_FOR_GNOME], [test x$GNOMESAVER = xyes])

# Check if we have kde screensaver support
if test "$KDE_CONFIG" != "None"; then
    for kssdir in `$KDE_CONFIG --path services | $SED -e 's/.*://'` ; do
        KDESAVER=yes
        KDE_DESKTOP_DIR=`echo "$kssdir/ScreenSavers" | $SED 's/\/\//\//g'`
    done
fi
AM_CONDITIONAL([INSTALL_FOR_KDE], [test x$KDESAVER = xyes])

# Check for the X environment
AC_PATH_XTRA()

# Check for GLU, uses X environment if available (requires g++)
AX_CHECK_GLU()

# Check for GL, uses X enviroment if available
AX_CHECK_GL()

# Check we have at least SDL 1.2.0
AM_PATH_SDL([1.2.0])

# Check for SDL_image
AC_CHECK_LIB([SDL_image], [IMG_isPNG], [], [AC_MSG_ERROR([libSDL_image is required and cannot be found])], [$SDL_LIBS])

# Check for X11
AC_CHECK_LIB([X11], [XGetWindowAttributes], [], [AC_MSG_ERROR([libX11 is required and cannot be found])], [$SDL_LIBS])

# Set the Solarsystem texture directory location
AX_DEFINE_DIR([TEXTUREDIR], [datadir/$PACKAGE_NAME/textures], ["Texture Directory"])

# Substitutions
AC_SUBST([TEXTUREDIR])
AC_SUBST([GNOMESAVER])
AC_SUBST([GNOME_DESKTOP_DIR])
AC_SUBST([GNOME_LNK_DIR])
AC_SUBST([KDESAVER])
AC_SUBST([KDE_DESKTOP_DIR])

AC_CONFIG_FILES([Makefile
                src/Makefile
                src/textures/Makefile
                src/gnome/Makefile
                src/kde/Makefile
                doc/Makefile
                debug_configure
                ])

AC_OUTPUT()

# Careful here. Only messages are safe
AC_MSG_NOTICE([  Build intent summary follows...])
AS_IF([test "$GNOMESAVER" = "yes"],
      [AC_MSG_NOTICE([    Build $PACKAGE_NAME for gnome-screensaver: Yes])],
      [AC_MSG_NOTICE([    Build $PACKAGE_NAME for gnome-screensaver: No])])

AS_IF([test "$KDESAVER" = "yes"],
      [AC_MSG_NOTICE([    Build $PACKAGE_NAME for KDE4 screensaver: Yes])],
      [AC_MSG_NOTICE([    Build $PACKAGE_NAME for KDE4 screensaver: No])])

AC_MSG_NOTICE([    Build $PACKAGE_NAME for Xscreensaver: Yes (manual install required)])

AS_ECHO([Done.])

