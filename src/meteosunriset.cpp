//
// meteosunriset.cc -- program to compute sunrise and sunset for a given
//                     station
//
// (c) 2004 Dr. Andreas Mueller, Beratung und Entwicklung
//
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <printver.h>
#include <string>
#include <Configuration.h>
#include <MeteoException.h>
#include <StationInfo.h>
#include <Timelabel.h>
#include <Sun.h>
#include <unistd.h>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <mdebug.h>

namespace meteo {
namespace sunriset {

void	usage(const char *progname) {
	std::cout << "find sunrise and sunset times" << std::endl;
	std::cout << "usage:" << std::endl;
	std::cout << "   " << progname << " [ options ] [ datestamps ]"
		<< std::endl;
	std::cout << "options:" << std::endl;
	std::cout << " -d,--debug          enable debug messages" << std::endl;
	std::cout << " -h,-?,--help        show this help message and exit"
		<< std::endl;
	std::cout << " -e,--elevation=<e>  elevation of the horizon"
		<< std::endl;
	std::cout << " -f,--config=<c>     use configuration file <f>"
		<< std::endl;
	std::cout << " -s,--station=<s>    use station <s>"
		<< std::endl;
	std::cout << " -F,--format=<f>     use the format string <f> for output"
		<< std::endl;
	std::cout << " -V,--version        show version of this program"
		<< std::endl;
	std::cout << " -L,--longitude=<L>  the longitude if no station given"
		<< std::endl;
	std::cout << " -l,--latitude=<l>   the latitude if no station given"
		<< std::endl;
}

static struct option	options[] = {
{ "debug",		no_argument,			NULL,		'd' },
{ "help",		no_argument,			NULL,		'h' },
{ "elevation",		required_argument,		NULL,		'e' },
{ "station",		required_argument,		NULL,		's' },
{ "config",		required_argument,		NULL,		'f' },
{ "format",		required_argument,		NULL,		'F' },
{ "latitude",		required_argument,		NULL,		'l' },
{ "longitude",		required_argument,		NULL,		'L' },
{ "version",		no_argument,			NULL,		'V' },
{ NULL,			0,				NULL,		 0  }
};

static std::string	formatstring("%H:%M");

static void	process(Sun& thesun, const time_t when) {
	// compute sunrise and sunset times
	time_t	sunrise	= thesun.sunrise(when);
	time_t	sunset	= thesun.sunset(when);

	// format the strings for output
	struct tm	*tp = localtime(&sunrise);
	char	buffer[1024];
	strftime(buffer, sizeof(buffer), formatstring.c_str(), tp);
	printf("%s ", buffer);
	tp = localtime(&sunset);
	strftime(buffer, sizeof(buffer), formatstring.c_str(), tp);
	printf("%s\n", buffer);
}

// main function for sunriset program
int	main(int argc, char *argv[]) {
	std::string	station;
	std::string	conffilename(METEOCONFFILE);
	double		elevation = -0.8333;
	int		c;
	double	longitude = 0;
	double	latitude = 0;

	// parse command line
	int	longindex;
	while (EOF != (c = getopt_long(argc, argv, "dh?e:s:f:F:V", options,
		&longindex)))
		switch (c) {
		case 'd':
			debug++;
			break;
		case 'e':
			elevation = std::stod(optarg);
			break;
		case 's':
			station = std::string(optarg);
			break;
		case 'f':
			conffilename = std::string(optarg);
			break;
		case 'F':
			formatstring = std::string(optarg);
			break;
		case 'V':
			fprintver(stdout);
			exit(EXIT_SUCCESS);
			break;
		case 'h':
			usage(argv[0]);
			return EXIT_SUCCESS;
		case 'l':
			latitude = std::stod(optarg);
			break;
		case 'L':
			longitude = std::stod(optarg);
			break;
		}

	// get the latitude and longitude from the database
	if (!station.empty()) {
		// open the configuration file
		Configuration	conf(conffilename);

		StationInfo	si(station);
		longitude = si.getLongitude();
		latitude = si.getLatitude();
	}
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "longitude: %.4f, latitude: %.4f\n",
		longitude, latitude);

	// create a Sun object
	Sun	thesun(longitude, latitude, elevation);

	// if there are no command line arguments, use current time
	if (optind >= argc) {
		time_t	now;
		time(&now);
		printf("today ");
		process(thesun, now);
		return EXIT_SUCCESS;
	}

	// the remaining arguments are a list of datestamps
	for (int i = optind; i < argc; i++) {
		// get the time
		printf("%-10.10s ", argv[i]);
		Timelabel	tl(argv[i]);

		// process this timelabel
		process(thesun, tl.getTime());
	}
	return EXIT_SUCCESS;
}

} // namespace sunriset 
} // namespace meteo

int	main(int argc, char *argv[]) {
	try {
		return meteo::sunriset::main(argc, argv);
	} catch (meteo::MeteoException& me) {
		fprintf(stderr, "MeteoException in meteosunriset: %s/%s\n",
			me.getReason().c_str(), me.getAddinfo().c_str());
	}
	return EXIT_FAILURE;
}
