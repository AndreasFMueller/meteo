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

	// create a socket
	_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (_socket < 0) {
		std::string msg = stringprintf("cannot create socket: %s",
			strerror(errno));
		mdebug(LOG_ERR, MDEBUG_LOG, 0, "%s", msg.c_str());
		throw MeteoException(msg, "");
	}

	// connect to the destination address
	if (connect(_socket, (struct sockaddr *)&_destination,
		sizeof(_destination)) < 0) {
		std::string	msg = stringprintf("cannot connect: %s",
			strerror(errno));
		mdebug(LOG_ERR, MDEBUG_LOG, 0, "%s", msg.c_str());
		throw MeteoException(msg, "");
	}
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

	// XXX collect the data into a UPD packet structure
	out << "rtOutsideTemp=21.2,";
	out << "rtOutsideHum=79,";
	out << "rtWindSpeed=13,";
	out << "rtWindAvgSpeed=5,";
	out << "rtRainRate=3";

	//

	// inspect the packet
	std::string	packet = out.str();
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "packet length: %d", packet.size());

	// send the packet
	if (sendto(_socket, packet.data(), packet.size(), 0,
		(struct sockaddr *)&_destination,
		sizeof(_destination)) != packet.size()) {
		std::string	msg = stringprintf("cannot send: %s",
			strerror(errno));
		throw MeteoException(msg, "");
	}
}

} // namespace meteo
