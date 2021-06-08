/*
 * UdpOutlet.cc -- send data as UDP packets
 *
 * (c) 2021 Prof Dr Andreas Müller
 */
#include <UdpOutlet.h>
#include <mdebug.h>
#include <netdb.h>
#include <MeteoException.h>
#include <Format.h>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <StationInfo.h>
#include <SensorStationInfo.h>
#include <Field.h>
#include <set>

namespace meteo {

/**
 * \brief Construct a UDP outlet class instance
 *
 * \param hostname	the destination host name
 * \param port		the destination port
 */
UdpOutlet::UdpOutlet(const std::string& stationname,
	const std::string& hostname, unsigned short port)
	: _stationname(stationname), _hostname(hostname), _port(port) {
	setup();
	_names.insert(std::make_pair(
		std::string("temperature"), std::string("rtOutsideTemp")));
	_names.insert(std::make_pair(
		std::string("humidity"), std::string("rtOutsideHum")));
	_names.insert(std::make_pair(
		std::string("wind"), std::string("rtWindAvgSpeed")));
	_names.insert(std::make_pair(
		std::string("windgust"), std::string("rtWindSpeed")));
	_names.insert(std::make_pair(
		std::string("rainrate"), std::string("rtRainRate")));
}

/**
 * \brief Destroy the UdpOutlet
 */
UdpOutlet::~UdpOutlet() {
	close(_port);
}

/**
 * \brief setup method to construct the socket
 */
void	UdpOutlet::setup() {
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "setting up connection to %s:%uh",
		hostname().c_str(), port());
	// try the hostname being an address
	if (INADDR_NONE == (_destination.sin_addr.s_addr
		= inet_addr(hostname().c_str()))) {
		// resolve hostname
		struct hostent	*hp = gethostbyname(hostname().c_str());
		if (hp == NULL) {
			std::string	msg
				= stringprintf("cannot resolve '%s': %s",
					hostname().c_str(), strerror(errno));
			mdebug(LOG_ERR, MDEBUG_LOG, 0, "%s", msg.c_str());
			throw MeteoException(msg, "");
		}
		memcpy(&_destination.sin_addr.s_addr, hp->h_addr,
			sizeof(_destination.sin_addr.s_addr));
	}

	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "destination address: %s",
		inet_ntoa(_destination.sin_addr));

	// set the port number
	_destination.sin_port = htons(_port);
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "port: %u", _port);

	// create a socket
	_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (_socket < 0) {
		std::string msg = stringprintf("cannot create socket: %s",
			strerror(errno));
		mdebug(LOG_ERR, MDEBUG_LOG, 0, "%s", msg.c_str());
		throw MeteoException(msg, "");
	}
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "udp socket %d", _socket);
}

/**
 * \brief Send a packet with the current data
 *
 * \param timekey	the timekey for which this packet is valid
 */
void	UdpOutlet::flush(const time_t timekey) {
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "sending a packet");
	// use ostringstream to collect the data
	std::ostringstream	out;

	// we need a way to resolve the names
	StationInfo	si(stationname());
	int	stationid = si.getId();
	SensorStationInfo	ssi(stationname(), "outside");
	int	sensorid = ssi.getId();
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "station=%d, sensor=%d", stationid,
		sensorid);

	// build a set of sensor ids
	std::set<int>	sensorids;
	std::list<outlet_t>::iterator	i;
	for (i = batch.begin(); i != batch.end(); i++) {
		sensorids.insert(i->sensorid);
	}

	// collect the data into a UPD packet structure
	std::set<int>::iterator	j;
	std::string	separator;
	for (j = sensorids.begin(); j != sensorids.end(); j++) {
		SensorStationInfo	sensor(*j);
		Field	f;
		for (i = batch.begin(); i != batch.end(); i++) {
			if (i->sensorid == *j) {
				std::string	name = f.getName(i->fieldid);
				out << separator;
				separator = ",";
				std::map<std::string,std::string>::const_iterator n = _names.find(name);
				if (n != _names.end()) {
					out << n->second << "=" << i->value;
				}
			}
		}
	}

	// inspect the packet
	std::string	packet = out.str();
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "packet length: %d", packet.size());
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "packet content: '%s'",
		packet.c_str());

	// send the packet
	if (sendto(_socket, packet.data(), packet.size(), 0,
		(struct sockaddr *)&_destination,
		sizeof(_destination)) != packet.size()) {
		std::string	msg = stringprintf("cannot send: %s",
			strerror(errno));
//		throw MeteoException(msg, "");
	}
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "packet sent");
}

} // namespace meteo
