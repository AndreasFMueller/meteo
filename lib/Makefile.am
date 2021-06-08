#
# Makefile.am -- generate a portable Makefile for the meteo interface package
#
# (c) 2001-2021 Prof. Dr. Andreas Mueller
#

lib_LTLIBRARIES = libmeteo.la

METEOCONFFILE = \"@METEOCONFFILE@\"
AM_CPPFLAGS = -DMETEOCONFFILE=$(METEOCONFFILE) @LIBXML_CFLAGS@

VERSION_FILE = ../include/meteo_vers.h

libmeteo_la_LDFLAGS = -version-info `../shtool version -d libtool $(VERSION_FILE)`

libmeteo_la_SOURCES =							\
	Averager.cc							\
	Axis.cc								\
	BarometerReader.cc						\
	BasicConverter.cc						\
	BasicPacketReader.cc						\
	BasicRecorder.cc						\
	BasicValue.cc							\
	BasicValueFactory.cc						\
	BytePacketReader.cc						\
	Calibrator.cc							\
	Channel.cc							\
	ChannelFactory.cc						\
	ChunkDumper.cc							\
	Color.cc							\
	Configuration.cc						\
	Converter.cc							\
	ConverterFactory.cc						\
	Daemon.cc							\
	DataRecorder.cc							\
	Datarecord.cc							\
	Dataset.cc							\
	Datasink.cc							\
	DavisStation.cc							\
	Derivedvalue.cc							\
	Dewpoint.cc							\
	Dimension.cc							\
	Energy.cc							\
	ExtraTemperatureReader.cc					\
	FQField.cc							\
	FQField_internals.cc						\
	Field.cc							\
	Field_internals.cc						\
	FileChannel.cc							\
	Format.cc							\
	Frame.cc							\
	GraphPoint.cc							\
	GraphWindow.cc							\
	Graphics.cc							\
	GroWeather.cc							\
	Health.cc							\
	Heatindex.cc							\
	HumidityConverter.cc						\
	HumidityRecorder.cc						\
	HumidityValue.cc						\
	ImageMap.cc							\
	Label.cc							\
	LastRecorder.cc							\
	LeafwetnessConverter.cc						\
	LeafwetnessRecorder.cc						\
	LeafwetnessValue.cc						\
	Level.cc							\
	LevelConverter.cc						\
	LevelRecorder.cc						\
	LevelValue.cc							\
	Lock.cc								\
	MapArea.cc							\
	Mapfile.cc							\
	MeteoException.cc						\
	MinmaxRecorder.cc						\
	MoistureConverter.cc						\
	MoistureRecorder.cc						\
	MoistureValue.cc						\
	MsgCommon.cc							\
	MsgDequeuer.cc							\
	MsgQueuer.cc							\
	NibblePacketReader.cc						\
	OldDavisStation.cc						\
	OldTemperatureReader.cc						\
	OldWindReader.cc						\
	Outlet.cc							\
	PacketReader.cc							\
	PacketReaderFactory.cc						\
	Pidfile.cc							\
	Point.cc							\
	PressureConverter.cc						\
	PressureRecorder.cc						\
	PressureValue.cc						\
	Query.cc							\
	QueryOutlet.cc							\
	QueryProcessor.cc						\
	QueryResult.cc							\
	Rain.cc								\
	RainConverter.cc						\
	RainRate.cc							\
	RainRateConverter.cc						\
	RainRateReader.cc						\
	RainRateRecorder.cc						\
	RainReader.cc							\
	RainRecorder.cc							\
	ReaderInfo.cc							\
	Recorder.cc							\
	RecorderFactory.cc						\
	Rectangle.cc							\
	Scale.cc							\
	SensorStation.cc						\
	SensorStationInfo.cc						\
	SerialChannel.cc						\
	ShortPacketReader.cc						\
	SolarConverter.cc						\
	SolarRecorder.cc						\
	SolarValue.cc							\
	Station.cc							\
	StationFactory.cc						\
	StationInfo.cc							\
	Sun.cc								\
	SunData.cc							\
	TcpChannel.cc							\
	Tdata.cc							\
	TemperatureConverter.cc						\
	TemperatureReader.cc						\
	TemperatureRecorder.cc						\
	TemperatureValue.cc						\
	TimeConverter.cc						\
	TimeReader.cc							\
	TimeRecorder.cc							\
	TimeValue.cc							\
	Timelabel.cc							\
	Timestamp.cc							\
	Timeval.cc							\
	TransmitterConverter.cc						\
	TransmitterRecorder.cc						\
	TransmitterStatus.cc						\
	UVConverter.cc							\
	UVReader.cc							\
	UVRecorder.cc							\
	UVValue.cc							\
	Value.cc							\
	ValueFactory.cc							\
	VantagePro.cc							\
	VantageProWindReader.cc						\
	Vector.cc							\
	VoltageConverter.cc						\
	VoltageReader.cc						\
	VoltageRecorder.cc						\
	VoltageValue.cc							\
	WMII.cc								\
	WMIIWindReader.cc						\
	Wind.cc								\
	WindConverter.cc						\
	WindRecorder.cc							\
	WindSpeed.cc							\
	Windchill.cc							\
	XmlOutlet.cc							\
	XmlOutletDelegate.cc						\
	XmlOutletFactory.cc						\
	crc.c								\
	mdebug.c							\
	printver.c

EXTRA_DIST = README README.sunriset
