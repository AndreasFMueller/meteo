/*
 * meteolabel.cc -- utility program to convert timestamps into labels
 *                  and navigating up and down
 *
 * (c) 2003 Dr. Andreas Mueller, Beratung und Entwicklung
 */
#include <Timelabel.h>
#include <MeteoException.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

namespace meteo {
namespace label {

static struct option	options[] = {
{ "timestamp",		no_argument,		NULL,		's' },
{ "ctime",		no_argument,		NULL,		'c' },
{ "title",		no_argument,		NULL,		't' },
{ "label",		no_argument,		NULL,		'L' },
{ NULL,			0,			NULL,	 	 0  }
};

static int	main(int argc, char *argv[]) {
	int	c;
	bool	outtime = false, outctime = false, outlabel = true,
		outtitle = false;

	// read the command line
	int	longindex;
	while (EOF != (c = getopt_long(argc, argv, "scLt", options,
		&longindex)))
		switch (c) {
		case 's':
			// output as timestamp
			outtime = true;
			break;
		case 'c':
			// output ctime string
			outctime = true;
			break;
		case 't':
			// output as string suitable for a title
			outtitle = true;
			break;
		case 'L':
			// output as a label
			outlabel = false;
			break;
		}

	// there must be at least one more arguments
	if (optind >= argc) {
		throw MeteoException("must specify label argument", "");
	}

	// next argument is a label
	std::string		label = argv[optind++];
	Timelabel	tl;
	try {
		tl = Timelabel(label);
	} catch (...) {
		std::cerr << "cannot convert label '" << label << "'"
			<< std::endl;
		exit(EXIT_FAILURE);
	}

	// following arguments are operators
	for (int i = optind; i < argc; i++) {
		if (!strcmp(argv[i], "up")) {
			tl = tl.up();
		}
		if (!strcmp(argv[i], "upup")) {
			tl = tl.upup();
		}
		if (!strcmp(argv[i], "upupup")) {
			tl = tl.upupup();
		}
		if (!strcmp(argv[i], "down")) {
			tl = tl.down();
		}
		if (!strcmp(argv[i], "downdown")) {
			tl = tl.downdown();
		}
		if (!strcmp(argv[i], "downdowndown")) {
			tl = tl.downdowndown();
		}
		if (!strcmp(argv[i], "previous")) {
			tl = tl.previous();
		}
		if (!strcmp(argv[i], "next")) {
			tl = tl.next();
		}
	}

	// output in the desired format
	if (outtime)
		std::cout << tl.getTime() << std::endl;
	if (outctime)
		std::cout << tl.getCtime();	// no endl, ctime includes one
	if (outlabel)
		std::cout << tl.getString() << std::endl;
	if (outtitle)
		std::cout << tl.getTitle() << std::endl;

	exit(EXIT_SUCCESS);
}

} // namespace label
} // namespace meteo

int	main(int argc, char *argv[]) {
	try {
		meteo::label::main(argc, argv);
	} catch (meteo::MeteoException& me) {
		fprintf(stderr, "MeteoException in meteolabel: %s/%s\n",
			me.getReason().c_str(), me.getAddinfo().c_str());
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
