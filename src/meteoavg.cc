/*
 * meteoavg.cc -- utility programm to compute averages and insert them into
 *                the database
 *
 * (c) 2001 Dr. Andreas Mueller, Beratung und Entwicklung
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <errno.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <Averager.h>
#include <Configuration.h>
#include <StationInfo.h>
#include <Timestamp.h>
#include <Daemon.h>
#include <printver.h>
#include <string>
#include <mdebug.h>
#include <MeteoException.h>
#include <getopt.h>

namespace meteo {
namespace average {

// the daemon usually waits for the next time stamp that leaves 30 when
// divided by 300, and computes adds all the averages that are required
// for that timestamp
static void	avg_daemon(const std::string& station) {
	Averager	avg(station);
	int	offset = avg.getOffset();
	time_t	now, next;

	while (1) {
		// we go through the following loop repeatedly because it
		// can happen that the sleep call is interrupted by a signal
		// in which case we restart it
		do {
			// determine current time
			time(&now);

			// find out when will be the next point in time
			// to compute averages for. In addition, we wait 30
			// seconds more, because that gives us the assurance
			// that the data in the sdata table will be ready
			next = now - (now % 300) + 330;

			// wait till this happens
			if (debug)
				mdebug(LOG_DEBUG, MDEBUG_LOG, 0,
					"next event at %-24.24s, in %d seconds",
					ctime(&next), (int)(next - now));
		} while (0 != sleep(next - now));
		// at this point we cake out of the sleep without an interrupt,
		// so the the time is now == next

		// find the point in the past which to compute the avgs, this
		// will normally be about 30 seconds back
		next -= next % 300;

		// compute the timekey that corresponds to this time
		time_t	timekey = next + offset; 

		// compute all necessary averages. We have to add offset 
		// to the first argument because the Averager will again
		// substract the same value. Previously, the timekey was
		// to indicate the end of the time interval, but now
		// it points to the beginning, so we have to substract the
		// interval length to get the same behaviour
		avg.add(timekey - 300, 300, false);
		avg.add(timekey - 1800, 1800, false);
		avg.add(timekey - 7200, 7200, false);
		avg.add(timekey - 86400, 86400, false);
	}
}

static void	usage(void) {
printf(
"usage: meteoavg [ -FdnVh? ] [ -l logurl ] [ -f conffile ] [ -p pidfile ] \\\n"
"       [ -i interval ] [ -r repeats ] -s station [ starttime [ endtime ] ]\n"
"computes averages of meteo data between starttime and endtime or for\n"
"repeats intervals. With no arguments run as a daemon that computes\n"
"averages whenever all the data from the station can reasonably be expected\n"
"to have been inserted in the database.\n"
"  -a,--all                   force all averages\n"
"  -l,--logurl=<logurl>       send log messages to this url\n"
"  -p,--pidfile=<pidfile>     write the process pid to the pidfile\n"
"  -F,--foreground            stay in foreground (for debugging)\n"
"  -s,--station=<stationname  station name, mandatory\n"
"  -d,--debug                 increase debug level\n"
"  -f,--config=<conffile>     read configuration from conffile\n"
"  -i,--interval=<interval>   compute averages for this interval (300, 1800,\n"
"                             7200 or 86400)\n"
"  -r,--repeats=<repeats>     number of averages to compute\n"
"  -n,--dryrun                just tell what you are about to do,\n"
"                             don't actually do it\n"
"  -V,--version               print version and exit\n"
"  -h,-?,--help               print this help and exit\n"
"starttime and endtime have format YYYYMMDDhhmmss\n"
);
}

static struct option	options[] = {
{ "all",		no_argument,		NULL,		'a' },
{ "config",		required_argument,	NULL,		'f' },
{ "debug",		no_argument,		NULL,		'd' },
{ "dryrun",		no_argument,		NULL,		'n' },
{ "foreground",		no_argument,		NULL,		'F' },
{ "help",		no_argument,		NULL,		'h' },
{ "interval",		required_argument,	NULL,		'i' },
{ "logurl",		required_argument,	NULL,		'l' },
{ "numavg",		required_argument,	NULL,		'r' },
{ "pid",		required_argument,	NULL,		'p' },
{ "station",		required_argument,	NULL,		's' },
{ "version",		no_argument,		NULL,		'V' },
{ NULL,			0,			NULL,		 0  }
};

static int	main(int argc, char *argv[]) {
	std::string	conffilename(METEOCONFFILE);
	std::string	station;
	int		c, naverages = -1, interval = 0;
	bool		foreground = false;
	bool		daemonmode = false;
	time_t		fromt, tot, t;
	bool		haveavg, average_fake = false, all = false;
	std::string	pidfileprefix("/var/run/meteoavg-");

	// parse the command line					
	int	longindex;
	while (EOF != (c = getopt_long(argc, argv, "adf:Fi:l:r:np:Vs:h?",
		options, &longindex)))
		switch (c) {
		case 'l':
			if (mdebug_setup("meteoavg", optarg) < 0) {
				fprintf(stderr, "%s: cannot init log\n",
					argv[0]);
				exit(EXIT_FAILURE);
			}
			break;
		case 's':
			station = std::string(optarg);
			break;
		case 'd':
			debug++;
			break;
		case 'a':
			all = true;
			if (debug)
				mdebug(LOG_DEBUG, MDEBUG_LOG, 0,
					"all averages forced");
			break;
		case 'f':
			conffilename = std::string(optarg);
			break;
		case 'F':
			foreground = true;
			break;
		case 'i':
			interval = std::stoi(optarg);
			switch (interval) {
			case 300:
			case 1800:
			case 7200:
			case 86400:
				break;
			default:
				mdebug(LOG_CRIT, MDEBUG_LOG, 0,
					"illegal interval: %d", interval);
				exit(EXIT_FAILURE);
			}
			break;
		case 'p':
			pidfileprefix = optarg;
			break;
		case 'r':
			naverages = std::stoi(optarg);
			break;
		case 'n':
			average_fake = true;
			break;
		case 'V':
			fprintver(stdout);
			exit(EXIT_SUCCESS);
			break;
		case 'h':
		case '?':
			usage();
			exit(EXIT_SUCCESS);
		}

	// station name is required					
	if (station.empty()) {
		mdebug(LOG_CRIT, MDEBUG_LOG, 0, "no station specified, use -s");
		exit(EXIT_FAILURE);
	}
	StationInfo	si(station);

	// there should be zero to two more arguments: timestamps for	
	// the range for which we should compute averages		
	switch (argc - optind) {
	case 0:	// no time stamps means that we should run as a daemon	
		// and update the database automagically with the most	
		// recent averages					
		daemonmode = true;
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "running in daemon mode");
		break;
	case 1:
		if ((naverages < 0) || (interval == 0)) {
			mdebug(LOG_CRIT, MDEBUG_LOG, 0,
				"must specify repeats (-r) if if only first "
				"interval end given");
			exit(EXIT_FAILURE);
		}
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "start timestamp: %s",
			argv[optind]);
		fromt = Timestamp(argv[optind]).getTime();
		tot = fromt + interval * naverages;
		break;
	case 2:
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "interval %s - %s",
			argv[optind], argv[optind + 1]);
		fromt = Timestamp(argv[optind]).getTime();
		tot = Timestamp(argv[optind + 1]).getTime();
		break;
	default:
		mdebug(LOG_CRIT, MDEBUG_LOG, 0, "wrong number of arguments");
		exit(EXIT_FAILURE);
	}

	// read the configuration file					
	Configuration	conf(conffilename);

	// consistency checks						
	// interval must be set						
	if ((interval <= 0) && (!daemonmode)) {
		mdebug(LOG_CRIT, MDEBUG_LOG, 0, "interval (-i) not specified");
		exit(EXIT_FAILURE);
	}

	// connect to the database					
	if (daemonmode) {
		// become a daemon					
		Daemon	daemon(pidfileprefix, station, foreground);

		// start daemon mode for averages			
		avg_daemon(station);

		// if we ever get to this point, we exit
		exit(EXIT_SUCCESS);
	}

	// compute first timestamp, correcting it for the offset. The average
	// for timekey t actually uses data from t - offset to
	// t - offset + interval, so we must add the offset to t to get
	// the intervals we are interested in
	fromt -= (fromt + si.getOffset()) % interval;

	// compute updates in the range according to the command line parameters
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0,
		"start querying for averages between %d and %d",
		(int)fromt, (int)tot);
	// t is the start time of an interval, it is chosen such that t+offset
	// is a valid time key
	for (t = fromt; t <= tot; t += interval) {
		time_t	timekey = t + si.getOffset();
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0,
			"averages for time %d/%d, station '%s'",
			(int)t, interval, station.c_str());

		// create an Averager object
		Averager	avg(station);
		avg.setFake(average_fake);

		// create necessary averages
		if (!all)
			haveavg = avg.have(timekey, interval);
		else
			haveavg = false;
		if (!haveavg)
			avg.add(timekey, interval, all);
	}

	exit(EXIT_SUCCESS);
}

} // namespace average
} // namespace meteo

// main(argc, argv)	wrapper to catch MeteoExceptions thrown inside the
//			real main function. We need the other main function
//			as scope for the Daemon class, since the scope
//			determines how long the PID file will be around
int	main(int argc, char *argv[]) {
	try {
		meteo::average::main(argc, argv);
	} catch (meteo::MeteoException& me) {
		fprintf(stderr, "MeteoException in meteoavg: %s/%s\n",
			me.getReason().c_str(), me.getAddinfo().c_str());
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
