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
	Averager.cpp							\
	Axis.cpp							\
	BarometerReader.cpp						\
	BasicConverter.cpp						\
	BasicPacketReader.cpp						\
	BasicRecorder.cpp						\
	BasicValue.cpp							\
	BasicValueFactory.cpp						\
	BytePacketReader.cpp						\
	Calibrator.cpp							\
	Channel.cpp							\
	ChannelFactory.cpp						\
	ChunkDumper.cpp							\
	Color.cpp							\
	Configuration.cpp						\
	Converter.cpp							\
	ConverterFactory.cpp						\
	CurrentValue.cpp						\
	CurrentConverter.cpp						\
	Daemon.cpp							\
	DataRecorder.cpp						\
	Datarecord.cpp							\
	Dataset.cpp							\
	Datasink.cpp							\
	DavisStation.cpp						\
	Derivedvalue.cpp						\
	Dewpoint.cpp							\
	Dimension.cpp							\
	Energy.cpp							\
	EnergyConverter.cpp						\
	EnergyValue.cpp							\
	ExtraTemperatureReader.cpp					\
	FQField.cpp							\
	FQField_internals.cpp						\
	Field.cpp							\
	Field_internals.cpp						\
	FileChannel.cpp							\
	Format.cpp							\
	Frame.cpp							\
	GraphPoint.cpp							\
	GraphWindow.cpp							\
	Graphics.cpp							\
	GroWeather.cpp							\
	Health.cpp							\
	Heatindex.cpp							\
	HumidityConverter.cpp						\
	HumidityRecorder.cpp						\
	HumidityValue.cpp						\
	ImageMap.cpp							\
	Label.cpp							\
	LastRecorder.cpp						\
	LeafwetnessConverter.cpp					\
	LeafwetnessRecorder.cpp						\
	LeafwetnessValue.cpp						\
	Level.cpp							\
	LevelConverter.cpp						\
	LevelRecorder.cpp						\
	LevelValue.cpp							\
	Lock.cpp							\
	MapArea.cpp							\
	Mapfile.cpp							\
	MeteoException.cpp						\
	MinmaxRecorder.cpp						\
	MoistureConverter.cpp						\
	MoistureRecorder.cpp						\
	MoistureValue.cpp						\
	MsgCommon.cpp							\
	MsgDequeuer.cpp							\
	MsgQueuer.cpp							\
	NibblePacketReader.cpp						\
	OldDavisStation.cpp						\
	OldTemperatureReader.cpp					\
	OldWindReader.cpp						\
	Outlet.cpp							\
	PacketReader.cpp						\
	PacketReaderFactory.cpp						\
	Pidfile.cpp							\
	Point.cpp							\
	PowerConverter.cpp						\
	PowerRecorder.cpp						\
	PowerValue.cpp							\
	PressureConverter.cpp						\
	PressureRecorder.cpp						\
	PressureValue.cpp						\
	Query.cpp							\
	QueryOutlet.cpp							\
	QueryProcessor.cpp						\
	QueryResult.cpp							\
	Rain.cpp							\
	RainConverter.cpp						\
	RainRate.cpp							\
	RainRateConverter.cpp						\
	RainRateReader.cpp						\
	RainRateRecorder.cpp						\
	RainReader.cpp							\
	RainRecorder.cpp						\
	ReaderInfo.cpp							\
	Recorder.cpp							\
	RecorderFactory.cpp						\
	Rectangle.cpp							\
	Scale.cpp							\
	SensorStation.cpp						\
	SensorStationInfo.cpp						\
	SerialChannel.cpp						\
	ShortPacketReader.cpp						\
	SolarConverter.cpp						\
	SolarRecorder.cpp						\
	SolarValue.cpp							\
	Station.cpp							\
	StationFactory.cpp						\
	StationInfo.cpp							\
	Sun.cpp								\
	SunData.cpp							\
	TcpChannel.cpp							\
	Tdata.cpp							\
	TemperatureConverter.cpp					\
	TemperatureReader.cpp						\
	TemperatureRecorder.cpp						\
	TemperatureValue.cpp						\
	TimeConverter.cpp						\
	TimeReader.cpp							\
	TimeRecorder.cpp						\
	TimeValue.cpp							\
	Timelabel.cpp							\
	Timestamp.cpp							\
	Timeval.cpp							\
	TransmitterConverter.cpp					\
	TransmitterRecorder.cpp						\
	TransmitterStatus.cpp						\
	UdpOutlet.cpp							\
	UVConverter.cpp							\
	UVReader.cpp							\
	UVRecorder.cpp							\
	UVValue.cpp							\
	Value.cpp							\
	ValueFactory.cpp						\
	VantagePro.cpp							\
	VantageProWindReader.cpp					\
	Vector.cpp							\
	VoltageConverter.cpp						\
	VoltageReader.cpp						\
	VoltageRecorder.cpp						\
	VoltageValue.cpp						\
	WMII.cpp							\
	WMIIWindReader.cpp						\
	Wind.cpp							\
	WindConverter.cpp						\
	WindRecorder.cpp						\
	WindSpeed.cpp							\
	Windchill.cpp							\
	XmlOutlet.cpp							\
	XmlOutletDelegate.cpp						\
	XmlOutletFactory.cpp						\
	crc.c								\
	mdebug.c							\
	printver.c

EXTRA_DIST = README README.sunriset
