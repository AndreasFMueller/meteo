#
# Makefile.am -- generate a portable Makefile for the meteo interface package
#
# (c) 2001-2021 Prof. Dr. Andreas Mueller
#
METEOCONFFILE = \"@METEOCONFFILE@\"
AM_CPPFLAGS = -DMETEOCONFFILE=$(METEOCONFFILE) @LIBXML_CFLAGS@

EXTRA_DIST = packet.csv

LDADD = ../lib/libmeteo.la

bin_PROGRAMS =								\
	meteopoll							\
	meteoavg							\
	meteolast							\
	meteodequeue							\
	meteodraw							\
	meteolabel							\
	meteodbdump							\
	meteosensors							\
	meteosunriset							\
	meteomap							\
	meteopacket
 
meteoavg_SOURCES = meteoavg.cc
meteodequeue_SOURCES = meteodequeue.cc
meteopoll_SOURCES = meteopoll.cc
meteodraw_SOURCES = meteodraw.cc
meteolabel_SOURCES = meteolabel.cc
meteolast_SOURCES = meteolast.cc
meteodbdump_SOURCES = meteodbdump.cc
meteosensors_SOURCES = meteosensors.cc
meteosunriset_SOURCES = meteosunriset.cc
meteomap_SOURCES = meteomap.cc
meteopacket_SOURCES = meteopacket.cc

test:	meteopoll
	cp /dev/null /var/tmp/meteopoll.log
	./meteopoll --debug --debug --debug \
		--logurl=file:///var/tmp/meteopoll.log \
		--config=`pwd`/../conf/meteo.xml \
		--foreground --station=Altendorf2 \
		--map=/var/tmp/meteo.xml \
		--pidfile=`pwd`/meteopoll- --backend=msgqueue 

#
# draw standard graph
#
#		-f `pwd`/../conf/meteo.xml -I -p ../src/ 
testdraw:	meteodraw
	cp /dev/null /var/tmp/meteodraw.log
	DYLD_BIND_AT_LAUNCH=1 ./meteodraw -x -s Altendorf -d -d \
		-l file:///var/tmp/meteodraw.log \
		-f ../conf/meteo.xml -I -p ../src/ \
		-u "/browser/meteobrowser.php?lang=de" -I \
		-L d20040401 \
		-g Altendorf.temperature

testmap:	meteodraw
	DYLD_BIND_AT_LAUNCH=1 ./meteodraw -s Lajeado -d -d \
		-f ../conf/meteo.xml -I -p ../src/ \
		-u "/browser/meteobrowser.php?lang=de" -I \
		-L m200309 -m \
		-g Lajeado.rain 
	
#
# drawing high resolution graphs uses a different configuration file
#
testdraw60:	meteodraw
	cp /dev/null /var/tmp/meteodraw.log
	DYLD_BIND_AT_LAUNCH=1 ./meteodraw -s Gurin \
		-l file:///var/tmp/meteodraw.log -d -d \
		-f `pwd`/../conf/meteo-60.xml -I -p ../src/ \
		-u "/browser/meteobrowser.php?lang=de" \
		-L h2004022720 -g Gurin.temperature_inside

#
# test computing averages
#
testavg:	meteoavg
	cp /dev/null /var/tmp/meteoavg.log
	DYLD_BIND_AT_LAUNCH=1 ./meteoavg -s OBS \
		-d -l file:///var/tmp/meteoavg.log \
		-i 7200 -f `pwd`/../conf/meteo.xml -a \
		20040201000000 20040226000000

testavgd:	meteoavg
	cp /dev/null /var/tmp/meteoavg.log
	DYLD_BIND_AT_LAUNCH=1 ./meteoavg -sLajeado -p `pwd`/meteoavg- \
		-l file:///var/tmp/meteoavg.log -d \
		-f `pwd`/../conf/meteo.xml

#
# test dequeuer
#
testdequeue:	meteodequeue
	DYLD_BIND_AT_LAUNCH=1 ./meteodequeue \
		-p `pwd`/meteodequeue- \
		-d -f `pwd`/../conf/meteo.xml

#
# test the meteolast program
#
testlast:	meteolast
	DYLD_BIND_AT_LAUNCH=1 ./meteolast -x -d -w 1000000000 \
		-f `pwd`/../conf/meteo.xml Lajeado

#
# test the dump programm
#
testdump:	meteodbdump
	cp /dev/null /var/tmp/meteodump.log
	DYLD_BIND_AT_LAUNCH=1 ./meteodbdump -r -s Lajeado \
		-l file:///var/tmp/meteodump.log -d -b testdump- \
		-f `pwd`/../conf/meteo.xml avg sdata
	

#
# test the packet program
#
testpacket:	meteopacket
	DYLD_BIND_AT_LAUNCH=1 ./meteopacket --debug --station=Gurin \
		--server=miranda --config=meteo-test.xml
	DYLD_BIND_AT_LAUNCH=1 ./meteopacket --debug --station=Gurin \
		--server=miranda --config=meteo-test.xml --file=packet.csv
