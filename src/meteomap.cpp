//
// meteomap.cc
//
// (c) 2004 Dr. Andreas Mueller, Beratung und Entwicklung
//
#include <stdlib.h>
#include <stdio.h>
#include <Mapfile.h>
#include <iostream>
#include <mdebug.h>
#include <getopt.h>

namespace meteo {
namespace map {

void	usage(char *progname) {
printf("atomically read a map file and display it\n"
"usage:\n"
"%s [ -dh? ] <mapfilename>\n"
"options:\n"
" -d,--debug      increase debug level\n"
" -h,-?,--help    display this help message and exit\n"
"\n", progname);
}

static option	options[] = {
{ "debug",	no_argument,		NULL,		'd' },
{ "help",	no_argument,		NULL,		'h' },
{ NULL,		0,			NULL,		 0  }
};

int	main(int argc, char *argv[]) {
	int	c, longindex;
	while (EOF != (c = getopt_long(argc, argv, "dh?", options, &longindex)))
		switch (c) {
		case 'd':
			debug++;
			break;
		case 'h':
		case '?':
			usage(argv[0]);
			return EXIT_SUCCESS;
		}

	// make sure there is exactly one argument, the map file
	if (argc != 2) {
		fprintf(stderr, "wrong number of arguments: map file\n");
		exit(EXIT_FAILURE);
	}

	try {
		// open the mapfile
		Mapfile	m(argv[1], false);

		// read the map file atomically
		std::string	map = m.read();
		std::cout << map << std::endl;
	} catch (...) {
		std::cerr << "cannot read map file" << std::endl;
		exit(EXIT_FAILURE);
	}

	// that's it
	exit(EXIT_SUCCESS);
}

} // namespace map
} // namespace meteo

int	main(int argc, char *argv[]) {
	try {
		return meteo::map::main(argc, argv);
	} catch (const std::exception& x) {
		std::cerr << "terminated by exception: " << x.what();
		std::cerr << std::endl;
	} catch (...) {
		std::cerr << "terminated by unknown exception" << std::endl;
	}
	return EXIT_FAILURE;
}
