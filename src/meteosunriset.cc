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
#include <stdlib.h>
#include <getopt.h>

namespace meteo {
namespace sunriset {

static struct option	options[] = {
{ "elevation",		required_argument,		NULL,		'e' },
{ "station",		required_argument,		NULL,		's' },
{ "config",		required_argument,		NULL,		'f' },
{ "format",		required_argument,		NULL,		'F' },
{ "version",		no_argument,			NULL,		'V' },
{ NULL,			0,				NULL,		 0  }
};

// main function for sunriset program
void	main(int argc, char *argv[]) {
	std::string	station;
	std::string	conffilename(METEOCONFFILE);
	std::string	formatstring("%H:%M");
	double		elevation = -0.8333;
	int		c;

	// parse command line
	int	longindex;
	while (EOF != (c = getopt_long(argc, argv, "e:s:f:F:V", options,
		&longindex)))
		switch (c) {
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
		}

	// make sure the station was specified
	if (station.empty()) {
		throw MeteoException("must specify station", "");
	}

	// open the configuration file
	Configuration	conf(conffilename);

	// get the latitude and longitude from the database
	StationInfo	si(station);
	double	longitude = si.getLongitude();
	double	latitude = si.getLatitude();

	// create a Sun object
	Sun	thesun(longitude, latitude, elevation);

	// the remaining arguments are a list of datestamps
	for (int i = optind; i < argc; i++) {
		char	buffer[1024];
		// get the time
		printf("%-10.10s ", argv[i]);
		Timelabel	tl(argv[i]);

		// compute sunrise and sunset times
		time_t	sunrise	= thesun.sunrise(tl.getTime());
		time_t	sunset	= thesun.sunset(tl.getTime());

		// format the strings for output
		struct tm	*tp = localtime(&sunrise);
		strftime(buffer, sizeof(buffer), formatstring.c_str(), tp);
		printf("%s ", buffer);
		tp = localtime(&sunset);
		strftime(buffer, sizeof(buffer), formatstring.c_str(), tp);
		printf("%s\n", buffer);
	}
}

} // namespace sunriset 
} // namespace meteo

int	main(int argc, char *argv[]) {
	try {
		meteo::sunriset::main(argc, argv);
	} catch (meteo::MeteoException& me) {
		fprintf(stderr, "MeteoException in meteosunriset: %s/%s\n",
			me.getReason().c_str(), me.getAddinfo().c_str());
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
