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
 
meteoavg_SOURCES = meteoavg.cpp
meteodequeue_SOURCES = meteodequeue.cpp
meteopoll_SOURCES = meteopoll.cpp
meteodraw_SOURCES = meteodraw.cpp
meteolabel_SOURCES = meteolabel.cpp
meteolast_SOURCES = meteolast.cpp
meteodbdump_SOURCES = meteodbdump.cpp
meteosensors_SOURCES = meteosensors.cpp
meteosunriset_SOURCES = meteosunriset.cpp
meteomap_SOURCES = meteomap.cpp
meteopacket_SOURCES = meteopacket.cpp

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
		--pidfile=`pwd`/meteodequeue- \
		--debug --config=`pwd`/../conf/meteo.xml

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

#
# test sunrise/sunset program
#
testsunrise:	meteosunriset
	DYLD_BIND_AT_LAUNCH=1 ./meteosunriset --debug \
		--longitude=6.9 --latitude=47.06
	DYLD_BIND_AT_LAUNCH=1 TZ=JST ./meteosunriset --debug \
		--longitude=140.12 --latitude=35.74
	
