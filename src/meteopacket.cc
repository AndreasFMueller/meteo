/*
 * meteopacket.cc
 *
 * (c) 2021 Prof Dr Andreas Müller
 */
#include <MeteoException.h>
#include <iostream>
#include <mdebug.h>
#include <getopt.h>
#include <Configuration.h>
#include <StationInfo.h>
#include <SensorStationInfo.h>
#include <UdpOutlet.h>
#include <FQField.h>

namespace meteo {
namespace packet {

static void	usage() {
printf(
"usage:\n"
"meteopacket [ -dh? ] [ -l logurl ] [ -P port ] [ -c config ] -S servername -s stationame\n"
"options:\n"
"  -d,--debug                   enable debugging\n"
"  -h,-?,--help                 show this help message and exit\n"
"  -c,--config=<config>         use the configfile <config>\n"
"  -l,--logurl=<logurl>         send log messages to <logurl>\n"
"  -P,--port=<port>             the port to send the packet to\n"
"  -S,--server=<servername>     the name or IP address of the server\n"
"  -s,--station=<stationname>   the name weather station\n");
}

static struct option	options[] = {
{	"config",	required_argument,	NULL,		'c' },
{	"debug",	no_argument,		NULL,		'd' },
{	"help",		no_argument,		NULL,		'h' },
{	"logurl",	required_argument,	NULL,		'l' },
{	"port",		required_argument,	NULL,		'p' },
{	"station",	required_argument,	NULL,		's' },
{	"server",	required_argument,	NULL,		'S' },
{	NULL,		0,			NULL,		 0  }
};

static std::string	stationname;
static int	stationid;

static void	send(Outlet *outlet,
	const std::string& sensorname,
	const std::string& fieldname,
	double value, const std::string& unit) {

	// build the name
	std::string	fqfieldname = stationname;
	fqfieldname.append(".");
	fqfieldname.append(sensorname);
	fqfieldname.append(".");
	fqfieldname.append(fieldname);
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "fq fieldname: %s",
		fqfieldname.c_str());

	// lookup 
	FQField		fqfield;
	int	fieldid = fqfield.getFieldid(fqfieldname).mfieldid;

	// create the sensor station info
	SensorStationInfo	ssi(stationname, sensorname);
	int	sensorid = ssi.getId();

	// update the outlet with the information
	outlet->send(sensorid, fieldid, value, unit);
}

static int	main(int argc, char *argv[]) {
	std::string	conffile(METEOCONFFILE);
	std::string	logurl("file:///-");
	std::string	servername;
	int	port = 5002;

	// parse command line
	int	longindex;
	int	c;
	while (EOF != (c = getopt_long(argc, argv, "c:dh?l:s:S:p:",
		options, &longindex)))
		switch (c) {
		case 'c':
			conffile = std::string(optarg);
			break;
		case 'd':
			debug++;
			break;
		case 'h':
		case '?':
			usage();
			return EXIT_SUCCESS;
		case 'l':
			logurl = std::string(optarg);
			break;
		case 'p':
			port = std::stoi(optarg);
			break;
		case 's':
			stationname = std::string(optarg);
			break;
		case 'S':
			servername = std::string(optarg);
			break;
		}

	// set up the logging
	mdebug_setup("meteopacket", logurl.c_str());

	// check consistency of data
	if (stationname.empty()) {
		std::cerr << "empty station name" << std::endl;
		mdebug(LOG_CRIT, MDEBUG_LOG, 0, "station name not set");
		exit(EXIT_FAILURE);
	}
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "station name: %s",
		stationname.c_str());
	if (servername.empty()) {
		std::cerr << "empty server name" << std::endl;
		mdebug(LOG_CRIT, MDEBUG_LOG, 0, "server name not set");
		exit(EXIT_FAILURE);
	}
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "servername name: %s",
		servername.c_str());

	// open the configuration file
	Configuration	conf(conffile);

	// create the station info
	StationInfo	si(stationname);
	stationid = si.getId();

	// create the UDP outlet
	UdpOutlet	*outlet = new UdpOutlet(stationname, servername, port);

	// add data to the packet
	send(outlet, "inside", "temperature", 22.5, "C");
	send(outlet, "inside", "humidity", 62.5, "%");
	send(outlet, "inside", "barometer", 1020, "hPa");
	send(outlet, "outside", "temperature", 2.5, "C");
	send(outlet, "outside", "humidity", 82.5, "%");
	send(outlet, "outside", "wind", 15., "m/s");
	send(outlet, "outside", "winddir", 47, "deg");
	send(outlet, "outside", "windgust", 25., "m/s");
	send(outlet, "outside", "rainrate", 3., "mm/h");

	// send a packet
	outlet->flush(0);

	return EXIT_SUCCESS;
}

} // namespace packet
} // namespace meteo

int	main(int argc, char *argv[]) {
	try {
		return meteo::packet::main(argc, argv);
	} catch (meteo::MeteoException& x) {
		std::cerr << "MeteoException in meteopacket: ";
		std::cerr << x.getReason() << "/" << x.getAddinfo();
		std::cerr << std::endl;
		return EXIT_FAILURE;
	}
}
