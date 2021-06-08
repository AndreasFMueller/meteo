#
# Makefile.am -- generate a portable Makefile for the meteo interface package
#
# (c) 2001-2021 Prof. Dr. Andreas Mueller
#
VERSION = @VERSION@

SUBDIRS = include lib src conf html browser db doc man support \
	wunderground gallery
BUILTSOURCES = README
EXTRA_DIST = shtool README.tmpl SETUP configureit BUGS UPGRADE

README: 	README.tmpl
	(figlet meteo-$(VERSION); cat README.tmpl) > README

