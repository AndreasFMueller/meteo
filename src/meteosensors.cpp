/*
 * meteosensors.cc -- report the names of the sensors a station knows about
 *
 * (c) 2003 Dr. Andreas Mueller, Beratung und Entwicklung
 */
#include <stdio.h>
#include <stdlib.h>
#include <StationFactory.h>
#include <MeteoException.h>

namespace meteo {
namespace sensors {

static void	knowntypes(FILE *out) {
	stringlist	nt = ReaderInfo::knownTypes();
	for (stringlist::iterator i = nt.begin(); i != nt.end(); i++) {
		if (i != nt.begin())
			fprintf(out, ", ");
		fprintf(out, "%s", i->c_str());
	}
	fprintf(out, "\n");
}

static void	usage(char *name) {
	fprintf(stderr, "usage: %s stationtype\n", name);
	fprintf(stderr, "\tdisplays the sensor names known to a station of this type\n");
	fprintf(stderr, "\tknown station types:\n\t");
	knowntypes(stderr);
	exit(EXIT_FAILURE);
}

static int	main(int argc, char *argv[]) {
	// this thing has only one argument, so if it is missing, we display
	// a usage message
	if (argc != 2)
		usage(argv[0]);
	
	// get the readers
	stationreaders_t	*srp = ReaderInfo::getReaders(argv[1]);
	if (NULL == srp) {
		fprintf(stderr, "station type %s unknown, known types are:\n",
			argv[1]);
		knowntypes(stderr);
		exit(EXIT_FAILURE);
	}

	// display the names and types
	do {
		printf("%s %s %s\n", srp->readername, srp->classname,
			srp->readerunit);
	} while ((++srp)->readername);
	exit(EXIT_SUCCESS);
}

} // namespace sensors
} // namespace meteo

int	main(int argc, char *argv[]) {
	try {
		meteo::sensors::main(argc, argv);
	} catch (meteo::MeteoException& me) {
		fprintf(stderr, "MeteoException in meteosensors: %s/%s\n",
			me.getReason().c_str(), me.getAddinfo().c_str());
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
