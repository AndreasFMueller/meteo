#
# Makefile.am -- generate a portable Makefile for the meteo interface package
#
# (c) 2001-2021 Prof. Dr. Andreas Mueller
#
EXTRA_DIST = Appendix.txt Techref.txt faq.txt vtechref.txt uds10 \
	architecture sensors graphing

uds10:	uds10.in
	nroff uds10.in | uniq > uds10
