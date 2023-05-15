AC_PREREQ([2.69])dnl
AC_REVISION([1.0])dnl

dnl Process this file with autoconf to produce a configure script.
AC_INIT([meteo],[0.9.21],[andreas.mueller@othello.ch])
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
LT_INIT
AC_CONFIG_SRCDIR([include/crc.h])
AM_INIT_AUTOMAKE([-Wall])
AC_CONFIG_HEADERS(include/config.h)

dnl add support for libtool
AC_SUBST(LIBTOOL_DEPS)
dnl AC_CHECK_LIB(dl, dlopen)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_ARG_VAR(MSGFMT, [name of msgfmt program from gettext package])
AC_CHECK_PROG(MSGFMT, msgfmt, msgfmt)
AC_PATH_PROG(XSLTPROC, xsltproc, na)
if test "${XSLTPROC}" = na
then
	AC_MSG_ERROR([xsltproc binary not found])
fi
AC_SUBST(XSLTPROC)

dnl Checks for libraries.
AC_CHECK_LIB(nsl, gethostbyname)
AC_CHECK_LIB(socket, socket)
AC_CHECK_LIB(resolv, res_query)
AC_CHECK_LIB(m, atan2)

dnl the following libraries are required, so configure should fail if
dnl it does not find them

dnl check for the right mysql flags, this should be done with the mysql_config
dnl script
MYSQL_CONFIG=mysql_config
AC_MSG_CHECKING(looking for mysqlclient library)
if ${MYSQL_CONFIG} --libs > /dev/null 2>&1
then
	MYSQLVERS=`${MYSQL_CONFIG} --version`
	MYSQL_LIBS=`${MYSQL_CONFIG} --libs`
	MYSQL_CFLAGS=`${MYSQL_CONFIG} --cflags`
	AC_SUBST(MYSQL_LIBS)
	AC_SUBST(MYSQL_CFLAGS)
	AC_MSG_RESULT(${MYSQLVERS} found)
else
	AC_MSG_ERROR([Could not find libmysqlclient anywhere, check http://www.mysql.com/])
fi
CFLAGS="${MYSQL_CFLAGS} ${CFLAGS}"
CXXFLAGS="${MYSQL_CFLAGS} ${CXXFLAGS}"
LIBS="${MYSQL_LIBS} ${LIBS}"

dnl here we have a problem: gd uses freetype, so we really should use the
dnl gdlib-config script
AC_MSG_CHECKING(looking for gd library)
if pkg-config gdlib --exists > /dev/null 2>&1
then
	GDLIBVERS=`pkg-config gdlib --modversion`
	GDLIB_LIBS=`pkg-config gdlib --libs`
	GDLIB_CFLAGS=`pkg-config gdlib --cflags`
	AC_SUBST(GDLIB_LIBS)
	AC_SUBST(GDLIB_CFLAGS)
	AC_MSG_RESULT(${GDLIBVERS} found)
else
	AC_MSG_ERROR([Could not find libgd anywhere, check http://www.boutell.com/])
fi
CFLAGS="${GDLIB_CFLAGS} ${CFLAGS}"
CXXFLAGS="${GDLIB_CFLAGS} ${CXXFLAGS}"
LIBS="${GDLIB_LIBS} ${LIBS}"


dnl Checks for header files.
AC_FUNC_ALLOCA
AC_HEADER_DIRENT
AC_HEADER_SYS_WAIT

AC_CHECK_HEADERS([argz.h arpa/inet.h fcntl.h malloc.h memory.h netdb.h \
netinet/in.h stdio.h stdlib.h string.h strings.h sys/socket.h sys/time.h \
syslog.h signal.h termios.h unistd.h sys/msg.h sys/ipc.h sys/sem.h \
alloca.h errno.h math.h getopt.h])
AC_CHECK_TYPES([semun])
if test "$ac_cv_type_semun" = "no"; then
  AC_MSG_CHECKING(if sem.h defines semun)
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/types.h>
      #include <sys/ipc.h>
      #include <sys/sem.h>]], [[union semun s;]])],[ac_cv_type_semun=yes; AC_DEFINE(HAVE_SEMUN) AC_MSG_RESULT(yes)],[AC_MSG_RESULT(no)])
fi

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_STRUCT_TM

dnl Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_FORK
AC_FUNC_MKTIME
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([alarm gethostbyname gettimeofday memmove memset putenv select \
socket sqrt strcasecmp strchr strdup strerror strrchr tzset floor pow strtol])

dnl Check for the XML library
XML_CONFIG=xml2-config
AC_MSG_CHECKING(looking for xml2 library)
if ${XML_CONFIG} --libs > /dev/null 2>&1
then
	XMLVERS=`${XML_CONFIG} --version`
	LIBXML_LIBS=`${XML_CONFIG} --libs`
	LIBXML_CFLAGS=`${XML_CONFIG} --cflags`
	AC_SUBST(LIBXML_LIBS)
	AC_SUBST(LIBXML_CFLAGS)
	AC_MSG_RESULT(${XMLVERS} found)
else
	AC_MSG_ERROR([Could not find libxml anywhere, check ftp://xmlsoft.org/])
fi
CFLAGS="${LIBXML_CFLAGS} ${CFLAGS}"
CXXFLAGS="${LIBXML_CFLAGS} ${CXXFLAGS}"
LIBS="${LIBXML_LIBS} ${LIBS}"

dnl default configuration directory
METEOCONFFILE=${sysconfdir}/meteo.xml
AC_SUBST(METEOCONFFILE)

dnl set the directory for the installation of the browser
BROWSERDIR=""
AC_ARG_WITH(browser-dir,
	[  --with-browser-dir      directory to install the meteo browser])
BROWSERDIR="${with_browser_dir}"
AC_SUBST(BROWSERDIR)

dnl set the directory for the installation of the HTML pages
HTMLDIR=""
AC_ARG_WITH(html-dir,
	[  --with-html-dir         directory to install the HTML pages])
HTMLDIR="${with_html_dir}"
AC_SUBST(HTMLDIR)

CONFIGDATE=`date +"%B %d, %Y" | tr 'A-Z' 'a-z'`
AC_SUBST(CONFIGDATE)

AC_CONFIG_FILES([Makefile include/Makefile lib/Makefile src/Makefile
support/Makefile conf/Makefile doc/Makefile html/Makefile wunderground/Makefile
browser/Makefile browser/po/Makefile
db/Makefile man/Makefile gallery/Makefile man/meteo.1
html/news.xml html/install.xml])
AC_OUTPUT
