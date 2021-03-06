/*
 * meteopoll.cc -- successor to meteoloop, polls the station using the loop
 *                 command, and writes data to mysql database. Is aware of
 *                 nonmetric units
 *
 * (c) 2003 Dr. Andreas Mueller, Beratung und Entwicklung
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif
#include <Station.h>
#include <StationFactory.h>
#include <Configuration.h>
#include <MeteoException.h>
#include <Mapfile.h>
#include <XmlOutletFactory.h>
#include <QueryOutlet.h>
#include <UdpOutlet.h>
#include <Datasink.h>
#include <Daemon.h>
#include <mdebug.h>
#include <errno.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <fstream>
#include <iostream>
#include <printver.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif
#include <sys/resource.h>
#include <getopt.h>

#define	ALARMTIMEOUT	30

pid_t	clpid;
bool	allowfork = true;;

// signal handler to kill child when parent is sent a signal
static void	kill_child(int cause) {
	switch (cause) {
	case SIGTERM:
	case SIGINT:
		if (allowfork)
			kill(clpid, cause);
		mdebug(LOG_INFO, MDEBUG_LOG, 0, "caught signal, exiting");
		exit(EXIT_SUCCESS);
		break;
	case SIGHUP:
		mdebug(LOG_INFO, MDEBUG_LOG, 0,
			"caught SIGHUP, killing child");
		if (allowfork)
			kill(clpid, SIGTERM);
		break;
	}
	return;
}

namespace meteo {
namespace poll {

static void	loop(const std::string& station,
			const std::string& mapfilename,
			const stringlist& xmloutlets,
			const std::string& servername, int port) {
	// limit memory to 16MB so this process cannot monopolize the system
#define	memmax	0x1 << 25
	struct rlimit	l;
	getrlimit(RLIMIT_STACK, &l);	// stack segment
	if (l.rlim_max > memmax)
		l.rlim_max = memmax;
	setrlimit(RLIMIT_STACK, &l);

	getrlimit(RLIMIT_DATA, &l);	// data segment
	if (l.rlim_max > memmax)
		l.rlim_max = memmax;
	setrlimit(RLIMIT_DATA, &l);

	// create a new station object (this always returns something
	// connected)
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "starting new station '%s'",
		station.c_str());
	Station	*s = StationFactory().newStation(station);
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "station %s ready", station.c_str());

	// add the mapfile if mapfilename is set
	if (!mapfilename.empty()) {
		Mapfile	*mapfile = new Mapfile(mapfilename, true);
		mapfile->setStationname(station);
		s->addMapfile(mapfile);	// takes ownership
	}

	// if we have an XML file, create a XmlOutlet with a delegate
	for (stringlist::const_iterator i = xmloutlets.begin();
		i != xmloutlets.end(); i++) {
		if (!i->empty()) {
			mdebug(LOG_DEBUG, MDEBUG_LOG, 0,
				"preparing XML outlet to %s", i->c_str());
			XmlOutlet *xmloutlet = XmlOutletFactory::get(station,
				*i);
			s->addOutlet(xmloutlet); // takes ownership
		}
	}

	// create a data sink (new implementation uses QueryOutlet class)
	s->addOutlet(new QueryOutlet(station));

	// add udp outlet
	if (!servername.empty()) {
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "configure Rts2 server '%s'",
			servername.c_str());
		s->addOutlet(new UdpOutlet(station, servername, port));
	}

	// loop:
	Timeval	looptime; looptime.now();
	int	minute = looptime.getMinute();
	while (true) {
		// start the loop, expecting 10 data packets
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "request 10 packets");
		s->startLoop(10);

		while (s->expectMorePackets()) {
			mdebug(LOG_DEBUG, MDEBUG_LOG, 0,
				"wait for more packets");
			// reset the alarm timer: if reading the packet takes
			// longer than 10 seconds, the connection is probably
			// broken
			mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "resetting timer");
			alarm(ALARMTIMEOUT);

			// read a packet from the station
			std::string	p = s->getPacket();
			mdebug(LOG_DEBUG, MDEBUG_LOG, 0,
				"received packet of length %d", p.length());

			// update the data from the packet in the station
			s->update(p);

			// if the minute has increased by one since the last
			// update save the current record in the database and
			// reset the datarecord
			looptime.now();
			if (minute != looptime.getMinute()) {
				// send data to outlets
				s->sendOutlets(looptime.getTimekey());

				// reset and restart
				s->reset();
				minute = looptime.getMinute();
			}
		}
	}
}

static void	usage(void) {
	printf(
"usage: meteopoll [ -dFVh? ] [ -l logurl ] [ -f conffile ] [ -b prefs ] \\\n"
"       [ -p pidfile ] [ -m mapfile ] [ -x xmlfile ] -s stationname\n"
"   -d,--debug               increase debug level\n"
"   -F,--foreground          don't fork (used for debugging)\n"
"   -V,--version             print version and exit\n"
"   -h,-?,--help             print this help screen and exit\n"
"   -f,--config=<conffile>   use conffile, see meteo.xml(5)\n"
"   -s,--station=<station>   let this process connect to station named stationname\n"
"   -S,--server=<server>     send RTS2 UDP data packets to this host\n"
"   -b,--backend=<prefs>     add backend preference prefs (one of msgqueue,\n"
"                            mysql, file, debug)\n"
"   -p,--pidfile=<pidfile>   write the process pid to this file\n"
"   -P,--port=<port>         use this port for RTS2 udp data packets\n"
"   -m,--mapfile=<mapfile>   keep current sensor information in mapfile\n"
"   -x,--xml=<xmlfile>       send output to XML file as well\n");
}

static struct option	options[] = {
{ "backend",		required_argument,	NULL,		'b' },
{ "config",		required_argument,	NULL,		'f' },
{ "debug",		no_argument,		NULL,		'd' },
{ "foreground",		no_argument,		NULL,		'F' },
{ "help",		no_argument,		NULL,		'h' },
{ "logurl",		required_argument,	NULL,		'l' },
{ "mapfile",		required_argument,	NULL,		'm' },
{ "pidfile",		required_argument,	NULL,		'p' },
{ "port",		required_argument,	NULL,		'P' },
{ "server",		required_argument,	NULL,		'S' },
{ "station",		required_argument,	NULL,		's' },
{ "xml",		required_argument,	NULL,		'x' },
{ NULL,			0,			NULL,		 0  }
};

static int	main(int argc, char *argv[]) {
	std::string	conffile(METEOCONFFILE);
	std::string	logurl("file:///-");	// default logging to stderr
	std::string	station, mapfilename;
	stringlist	xmloutlets;
	stringlist	preferences;
	std::string	pidfileprefix;
	std::string	server;
	int		port = 5002;

	// parse command line
	int	c;
	int	longindex;
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "parsing command line options");
	while (EOF != (c = getopt_long(argc, argv, "dl:f:m:s:b:p:VFh?x:S:P:",
		options, &longindex))) {
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "processing option '%c'", c);
		switch (c) {
		case 'd':
			debug++;
			break;
		case 'l':
			logurl = std::string(optarg);
			break;
		case 'f':
			conffile = std::string(optarg);
			break;
		case 's':
			station = std::string(optarg);
			break;
		case 'S':
			server = std::string(optarg);
			break;
		case 'b':
			preferences.push_back(std::string(optarg));
			break;
		case 'p':
			pidfileprefix = std::string(optarg);
			break;
		case 'P':
			port = std::stoi(optarg);
			break;
		case 'V':
			fprintver(stdout);
			return EXIT_SUCCESS;
			break;
		case 'F':
			allowfork = false;
			break;
		case 'h':
		case '?':
			usage();
			return EXIT_SUCCESS;
		case 'm':
			mapfilename = std::string(optarg);
			break;
		case 'x':
			xmloutlets.push_back(std::string(optarg));
			break;
		default:
			mdebug(LOG_ERR, MDEBUG_LOG, 0,
				"option %c not implemented", c);
			break;
		}
	}

	// set up logging
	mdebug_setup("meteopoll", logurl.c_str());
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "set up logging to %s",
		logurl.c_str());

	// it is an error if station name or conf
	if (station.empty()) {
		fprintf(stderr, "%s: station name not set\n", argv[0]);
		mdebug(LOG_CRIT, MDEBUG_LOG, 0, "station name not set");
		exit(EXIT_FAILURE);
	}

	// pid file
	if (pidfileprefix.empty())
		pidfileprefix = "/var/run/meteopoll-";

	// fork if necessary
	Daemon	d(pidfileprefix, station, !allowfork);

	// open the configuration object
	Configuration	conf(conffile);
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "read configuration %s",
		conffile.c_str());

	// create the Datasink object based on the configuration and prefs
	if (0 == preferences.size()) {
		preferences.push_back("debug");
	}
	DatasinkFactory	dsf(preferences);

	// install signal handlers
	signal(SIGTERM, kill_child);
	signal(SIGINT, kill_child);
	signal(SIGHUP, kill_child);

	// loop: fork a process and wait for it to die, exit if the child
	// exits naturally
	int	status;
	bool	giveup = false;
	do {
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "start loop");

		// fork only if allowed to do so
		if (allowfork)
			clpid = fork();
		else
			clpid = 0;

		// if the fork is not possible, we have some serious problem
		if (clpid < 0) {
			mdebug(LOG_CRIT, MDEBUG_LOG, MDEBUG_ERRNO,
				"fork failed: %s", strerror(errno));
			exit(EXIT_FAILURE);
		}

		// the child simply starts the loop
		if (clpid == 0) {
			if (allowfork)
				mdebug(LOG_DEBUG, MDEBUG_LOG, 0,
					"starting new child");
			// reset signal dispositions so that child can easily
			// be killed
			signal(SIGHUP, SIG_DFL);
			signal(SIGTERM, SIG_DFL);
			signal(SIGINT, SIG_DFL);

			// start looping
			try {
				mdebug(LOG_DEBUG, MDEBUG_LOG, 0,
					"starting loop(%s)", station.c_str());
				loop(station, mapfilename, xmloutlets,
					server, port);
				mdebug(LOG_DEBUG, MDEBUG_LOG, 0,
					"return from loop");
			} catch (MeteoException& e) {
				mdebug(LOG_ERR, MDEBUG_LOG, 0,
					"MeteoException(%s, %s)", 
					e.getReason().c_str(),
					e.getAddinfo().c_str());
				// when we exit at this point, the parent
				// process will fork a new child (if there is a
				// a parent, that is), and it will know that we
				// had a problem
				exit(EXIT_FAILURE);
			}
			// exiting at this point means that for some reason loop
			// ended without error. This should not really happen,
			// but to tell the parent that something different happend
			// we exit with EXIT_SUCCESS
			exit(EXIT_SUCCESS);
		} // end of child code

		// here we wait for the child to terminate
		if (waitpid(clpid, &status, 0) < 0) {
			mdebug(LOG_ERR, MDEBUG_LOG, 0, "error during wait: %s",
				strerror(errno));
		} else {
			// here we should be a bit more verbose, so that it
			// becomes less difficult to find out what happened
			// to the child
			if (WIFEXITED(status)) {
				mdebug(LOG_DEBUG, MDEBUG_LOG, 0,
					"child %d exited with status %d",
					clpid, WEXITSTATUS(status));
				giveup = (WEXITSTATUS(status) == 0);
			}
			if (WIFSIGNALED(status)) {
				mdebug(LOG_DEBUG, MDEBUG_LOG, 0,
					"child %d killed by signal %d",
					clpid, WTERMSIG(status));
			}
			if (WIFSTOPPED(status)) {
				// this should definitely not happen, lets kill
				// the process and start a new one
				mdebug(LOG_DEBUG, MDEBUG_LOG, 0,
					"child %d stopped by signal %d",
					clpid, WSTOPSIG(status));
				kill(clpid, SIGKILL);
			}
			mdebug(LOG_CRIT, MDEBUG_LOG, 0,
				"child process died: %d", WEXITSTATUS(status));
		}
		// if there is a problem with the station, we should wait a
		// few seconds so the station can recover from the problem.
		// This also prevents meteopoll from looping. 
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "sleeping for 3 seconds to "
			"give station time to recover from error condition");
		sleep(3);
	} while (!giveup);

	// child exited ok, so we do the same
	return EXIT_SUCCESS ;
}

} // namespace poll
} // namespace meteo

// main(argc, argv)	the main function is only needed as a wrapper to
//			catch an Exception and print a nice error message
//			on standard error (nothing is logged in this case
//			as it is unlikely that logging has been initialized
//			already
int	main(int argc, char *argv[]) {
	try {
		return meteo::poll::main(argc, argv);
	} catch (meteo::MeteoException& me) {
		fprintf(stderr, "MeteoException in meteopoll: %s/%s\n",
			me.getReason().c_str(), me.getAddinfo().c_str());
	}
	return EXIT_FAILURE;
}
