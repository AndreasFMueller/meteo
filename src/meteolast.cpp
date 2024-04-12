/*
 * meteolast.c c -- get the most recent update to the database and format it
 *                  in an XML based format
 * 
 * (c) 2002 Dr. Andreas Mueller, Beratung und Entwicklung
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#include <unistd.h>
#include <string>
#include <list>
#include <mdebug.h>
#include <printver.h>
#include <Configuration.h>
#include <Query.h>
#include <QueryProcessor.h>
#include <MeteoException.h>
#include <Timestamp.h>
#include <iostream>
#include <getopt.h>

namespace meteo {
namespace last {

static void	usage(const char *progname) {
printf("retrieve recent meteo data from the database\n"
"usage:\n"
"%s [ -dxh?+HV ] [ -l logurl ] [ -f conffile ] [ -t time ] [ -w window ] stationname ...\n"
"options:\n"
" -d,--debug                 increase debug level\n"
" -x,--xml                   produce XML format output instead of plain text\n"
" -h,-?,--help               show this help message and exit\n"
" -V,--version               show version and exit\n"
" -f,--config=<confffile>    use <conffile> as configuration file\n"
" -t,--timekey=<timekey>     retrieve data before unix time spec <timekey>\n"
" -T,--timestamp=<timestamp> retrieve data before the timestamp <timestamp>\n"
" -+,--backwards             search backwards in time\n"
" -w,--window=<secs>         window length in seconds to search\n"
"\n", progname);
}

static struct option	options[] = {
{ "logurl",		required_argument,		NULL,		'l' },
{ "xmlheader",		no_argument,			NULL,		'H' },
{ "debug",		no_argument,			NULL,		'd' },
{ "config",		required_argument,		NULL,		'f' },
{ "version",		no_argument,			NULL,		'V' },
{ "help",		no_argument,			NULL,		'h' },
{ "timestamp",		required_argument,		NULL,		'T' },
{ "timekey",		required_argument,		NULL,		't' },
{ "xml",		no_argument,			NULL,		'x' },
{ "backwards",		no_argument,			NULL,		'+' },
{ "window",		required_argument,		NULL,		'w' },
{ NULL,			0,				NULL,		 0  }
};

static int	main(int argc, char *argv[]) {
	std::string	conffilename(METEOCONFFILE);
	std::string	stationname("undefined");
	std::string	logurl("file:///-");
	bool		xmloutput = false;
	bool		searchbackwards = true;
	bool		addxmlheader = false;
	time_t		timekey;
	int		c;
	int		window = 3600;

	// initialize the logging stuff
	mdebug_setup("meteolast", logurl.c_str());
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "set up logging to %s",
		logurl.c_str());

	// by default, we will work from the current time
	timekey = time(NULL);

	// parse command line
	int	longindex;
	while (EOF != (c = getopt_long(argc, argv, "l:df:Vh?t:T:+w:xH",
		options, &longindex)))
		switch (c) {
		case 'l':
			logurl = optarg;
			break;
		case 'H':
			addxmlheader = true;
			break;
		case 'd':
			debug++;
			break;
		case 'f':
			conffilename = optarg;
			break;
		case 'V':
			fprintver(stdout);
			exit(EXIT_SUCCESS);
			break;
		case 'h':
		case '?':
			usage(argv[0]);
			exit(EXIT_SUCCESS);
			break;
		case 'T':
			{
				Timestamp	ts(optarg);
				timekey = ts.getTime();
			}
			break;
		case 't':
			timekey = std::stoi(optarg);
			break;
		case 'x':
			xmloutput = true;
			break;
		case '+':
			searchbackwards = false;
			break;
		case 'w':
			window = std::stoi(optarg);
			break;
		}

	// log search base
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "base timekey %d: %s", timekey,
		ctime(&timekey));

	// create a list of station names from the remaining arguments
	std::list<std::string>	stationlist;
	for (int i = optind; i < argc; i++)
		stationlist.push_back(std::string(argv[i]));

	// create a configuration object from the config filename
	Configuration	conf(conffilename);

	// iterate through the station names
	for (std::list<std::string>::const_iterator i = stationlist.begin();
		i != stationlist.end(); i++) {
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "processing station %s",
			i->c_str());

		// create a QueryProcessor
		QueryProcessor	qp(false);

		// retrieve a record for this station
		Datarecord	r;
		if (searchbackwards)
			r = qp.lastRecord(timekey, *i, window);
		else
			r = qp.firstRecord(timekey, *i, window);

		// display in the right format
		if (xmloutput) {
			if (addxmlheader)
				std::cout << "<?xml version=\"1.0\"?>\n"
					<< "<meteolist>\n";
			std::cout << r.xml(*i) ;
			if (addxmlheader)
				std::cout << "</meteolist>\n";
		} else {
			std::cout << r.plain(*i);
		}
	}

	exit(EXIT_SUCCESS);
}

} // namespace last
} // namespace meteo

int	main(int argc, char *argv[]) {
	try {
		meteo::last::main(argc, argv);
	} catch (meteo::MeteoException &me) {
		fprintf(stderr, "MeteoException in meteolast: %s/%s\n",
			me.getReason().c_str(), me.getAddinfo().c_str());
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
