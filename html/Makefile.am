#
# Makefile.am -- generate a portable Makefile for the meteo interface package
#
# (c) 2001-2021 Prof. Dr. Andreas Mueller
#
HTML =	ack.html gallery.html prereq.html install.html util.html 	\
	news.html about.html config.html support.html
HTMLFILES = meteodata.html meteo-install.html meteo.css $(HTML)
IMAGES = meteo.jpg
TEXTFILES = ../README ../ChangeLog ../NEWS ../AUTHORS ../INSTALL \
	../COPYING ../SETUP
HTMLDIR = @HTMLDIR@
EXTRA_DIST = meteodata.html meteo-install.html meteo.css gallery.xml	\
	about.xml ack.xml config.xml news.xml.in prereq.xml util.xml 	\
	support.xml							\
	$(IMAGES) meteo.xsl $(HTML) gallery_template.xml 		\
	gallery.xml.in gallery.xsl buildgallery.sh

install-data-local:	html
	@if [ ! -z "$(HTMLDIR)" ];					\
	then								\
		echo "installing the HTML documentation in $(HTMLDIR)";	\
		mkdir -p "$(HTMLDIR)";					\
		cp $(HTMLFILES) $(TEXTFILES) $(IMAGES) "$(HTMLDIR)";	\
	else								\
		echo "info: HTML docs not installed, used --with-html-dir"; \
	fi

ack.html:	ack.xml
	xsltproc meteo.xsl $< > $@

gallery.html:	gallery.xml
	xsltproc meteo.xsl $< > $@

prereq.html:	prereq.xml
	xsltproc meteo.xsl $< > $@

install.html:	install.xml
	xsltproc meteo.xsl $< > $@

util.html:	util.xml
	xsltproc meteo.xsl $< > $@

news.html:	news.xml
	xsltproc meteo.xsl $< > $@

about.html:	about.xml
	xsltproc meteo.xsl $< > $@

config.html:	config.xml
	xsltproc meteo.xsl $< > $@

support.html:	support.xml
	xsltproc meteo.xsl $< > $@

html-local:	$(HTML)

gallery.xml:	gallery.xml.in gallery.xsl
	./buildgallery.sh
