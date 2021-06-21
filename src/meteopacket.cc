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
#include <Format.h>
#include <vector>
#include <fstream>
#include <sstream>

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
"  -f,--file=<csvfile>          data to use for sending\n"
"  -l,--logurl=<logurl>         send log messages to <logurl>\n"
"  -P,--port=<port>             the port to send the packet to\n"
"  -S,--server=<servername>     the name or IP address of the server\n"
"  -s,--station=<stationname>   the name weather station\n");
}

class entry {
public:
	std::string	sensor;
	std::string	field;
	double		value;
	std::string	unit;
	entry(const std::string _sensor, const std::string& _field,
		double _value, const std::string& _unit)
		: sensor(_sensor), field(_field), value(_value), unit(_unit) {
	}
	std::string	toString() const {
		return stringprintf("%s,%s,%d,%s",
			sensor.c_str(), field.c_str(), value, unit.c_str());
	}
};

std::vector<std::string>	split(const std::string& s, char delimiter) {
	std::vector<std::string>	result;
	std::string	s2 = s;
	while (1) {
		size_t	i = s2.find(delimiter);
		if (i == std::string::npos) {
			result.push_back(s2);
			return result;
		}
		result.push_back(s2.substr(0, i));
		s2 = s2.substr(i+1);
	}
}

static struct option	options[] = {
{	"config",	required_argument,	NULL,		'c' },
{	"debug",	no_argument,		NULL,		'd' },
{	"file",		required_argument,	NULL,		'f' },
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

static void	send(Outlet *outlet, const entry& entry) {
	send(outlet, entry.sensor, entry.field, entry.value, entry.unit);
}

class packetconfig : public std::list<entry> {
public:
	packetconfig(const std::string& filename) {
		std::istream	*in = new std::ifstream(filename.c_str());
		while (!in->eof()) {
			char	buffer[1024];
			in->getline(buffer, sizeof(buffer)-1);
			std::vector<std::string>	v = split(std::string(buffer), ',');
			if (v.size() == 4) {
				double	d = std::stod(v[2]);
				push_back(entry(v[0], v[1], d, v[3]));
			}
		}
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "%d entries", size());
		delete in;
	}
	packetconfig() { }
	void	sendall(Outlet *outlet) {
		std::list<entry>::const_iterator	i;
		for (i = begin(); i != end(); i++) {
			send(outlet, *i);
		}
	}
	std::string	toString() const {
		std::ostringstream	out;
		std::list<entry>::const_iterator	i;
		for (i = begin(); i != end(); i++) {
			out << i->toString() << std::endl;
		}
		return out.str();
	}
};

static int	main(int argc, char *argv[]) {
	std::string	conffile(METEOCONFFILE);
	std::string	logurl("file:///-");
	std::string	servername;
	std::string	csvfile;
	int	port = 5002;

	// parse command line
	int	longindex;
	int	c;
	while (EOF != (c = getopt_long(argc, argv, "c:df:h?l:s:S:p:",
		options, &longindex)))
		switch (c) {
		case 'c':
			conffile = std::string(optarg);
			break;
		case 'd':
			debug++;
			break;
		case 'f':
			csvfile = std::string(optarg);
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
	packetconfig	*entries = NULL;
	if (csvfile.size() > 0) {
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "open csv file %s",
			csvfile.c_str());
		entries = new packetconfig(csvfile);
	} else {
		entries = new packetconfig();
		entries->push_back(entry("iss", "temperature", 2.5, "C"));
		entries->push_back(entry("iss", "humidity", 82.5, "%"));
		entries->push_back(entry("iss", "wind", 15., "m/s"));
		entries->push_back(entry("iss", "winddir", 47, "deg"));
		entries->push_back(entry("iss", "rainrate", 3., "mm/h"));
		entries->push_back(entry("console", "temperature", 22.5, "C"));
		entries->push_back(entry("console", "humidity", 62.5, "%"));
		entries->push_back(entry("console", "barometer", 1020, "hPa"));
	}

	// sending all the entries
	entries->sendall(outlet);

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
