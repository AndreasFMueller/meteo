#
# Makefile.am -- generate a portable Makefile for the meteo interface package
#
# (c) 2001-2021 Prof. Dr. Andreas Mueller
#
BROWSERFILES = meteobrowser.php	   					\
	up.png upup.png upupup.png week.png month.png year.png 		\
	next.png previous.png
BROWSERDIR = @BROWSERDIR@

EXTRA_DIST = README.browser meteobrowser.inc $(BROWSERFILES)		\
	current.xsl current.cgi

SUBDIRS = po

install-data-local:
	@if [ ! -z "$(BROWSERDIR)" ];					\
	then								\
		echo "installing meteobrowser files in $(BROWSERDIR)";	\
		cp $(BROWSERFILES) ../html/meteo.css "$(BROWSERDIR)";	\
		cp meteobrowser.inc $(BROWSERDIR)/meteobrowser.inc.default; \
	else								\
		echo "warning: meteobrowser not installed, use --with-browser-dir"; \
	fi
