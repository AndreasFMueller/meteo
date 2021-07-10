/*
 * UdpOutlet.cc -- send data as UDP packets
 *
 * (c) 2021 Prof Dr Andreas Müller
 */
#include <UdpOutlet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
#include <string.h>
#include <Configuration.h>

namespace meteo {

std::string	UdpOutlet::fqname(const std::string& sensorname,
	const std::string& fieldname) const {
	std::string	result = _stationname;
	result.append(".");
	result.append(sensorname);
	result.append(".");
	result.append(fieldname);
	return result;
}

std::string	UdpOutlet::getSensorname(const std::string& stationname,
			const std::string& rts2sensor) {
	// build the Xpath for the configuration of the sensor
	std::string	xpath = stringprintf("/meteo/station[@name='%s']"
		"/sensors/sensor[@rts2sensor='%s']/@name",
		stationname.c_str(), rts2sensor.c_str());

	std::string	 result = Configuration().getString(xpath, "");
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "found sensor '%s' for '%s'",
		result.c_str(), rts2sensor.c_str());
	return result;
}

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

	// get inside and outside sensor names
	_inside_sensor_name = getSensorname(stationname, "inside");
	if (_inside_sensor_name.size() == 0) {
		std::string	msg("inside sensor not configured");
		mdebug(LOG_ERR, MDEBUG_LOG, 0, "%s", msg.c_str());
		throw MeteoException(msg, "");
	}
	_outside_sensor_name = getSensorname(stationname, "outside");
	if (_outside_sensor_name.size() == 0) {
		std::string	msg("outside sensor not configured");
		mdebug(LOG_ERR, MDEBUG_LOG, 0, "%s", msg.c_str());
		throw MeteoException(msg, "");
	}

	// build the Xpath for the configuration of the fields
	std::string	xpath = stringprintf("/meteo/station[@name='%s']/sensors/sensor/field[string-length(@rts2name)>0]/@rts2name", stationname.c_str());
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "search xpath: %s", xpath.c_str());

	// get the configuration
	std::list<xmlNodePtr> 	l = Configuration().getNodeList(xpath);
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "found %d paths", l.size());

	// iterate through the paths
	std::list<xmlNodePtr>::const_iterator	i;
	for (i = l.begin(); i != l.end(); i++) {
		std::string	xp = Configuration().getXPath(*i);
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "processing path '%s'",
			xp.c_str());

		std::string	rts2name(Configuration().getString(xp, ""));
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "rts2name: %s",
			rts2name.c_str());

		std::string	sensor = Configuration().getString(
					xp + std::string("/../../@name"), "");
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "sensor = %s", sensor.c_str());

		std::string	fieldname = Configuration().getString(
					xp + std::string("/.."), "");
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "fieldname = %s",
			fieldname.c_str());

		std::string	fq = fqname(sensor, fieldname);
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "fq = %s", fq.c_str());

		_names.insert(std::make_pair(fq, rts2name));
	}
	
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "%d names prepared", _names.size());
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
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "setting up connection to %s:%hu",
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
	SensorStationInfo	ossi(stationname(), _outside_sensor_name);
	int	outsidesensorid = ossi.getId();
	SensorStationInfo	issi(stationname(), _inside_sensor_name);
	int	insidesensorid = issi.getId();
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0,
		"station=%d, outside sensor=%d, inside sensor = %d", stationid,
		outsidesensorid, insidesensorid);

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
				std::string	key = stationname();
				key.append(".");
				if (i->sensorid == outsidesensorid) {
					key.append(_inside_sensor_name);
				}
				if (i->sensorid == insidesensorid) {
					key.append(_outside_sensor_name);
				}
				key.append(".");
				key.append(name);
				mdebug(LOG_DEBUG, MDEBUG_LOG, 0,
					"field name: '%s', key: '%s'",
					name.c_str(), key.c_str());


				std::map<std::string,std::string>::const_iterator n = _names.find(key);
				if (n != _names.end()) {
					out << separator;
					separator = ",";
					out << n->second << "=" << i->value;
				} else {
					mdebug(LOG_ERR, MDEBUG_LOG, 0,
						"key %s not found",
						key.c_str());
				}
			}
		}
	}

	// inspect the packet
	std::string	packet = out.str();
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "Rts2 packet length: %d",
		packet.size());
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "Rts2 packet content: '%s'",
		packet.c_str());

	// send the packet
	ssize_t	rc = sendto(_socket, packet.data(), packet.size(), 0,
		(struct sockaddr *)&_destination, sizeof(_destination));
	if (rc < 0) {
		std::string msg = stringprintf("cannot send Rts2 packet: %s",
			strerror(errno));
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "%s", msg.c_str());
	}
	if (packet.size() != (unsigned int)rc) {
		std::string	msg = stringprintf("wrong number of bytes sent:"
			" %lu != %ld", packet.size(), rc);
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "%s", msg.c_str());
	}
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "Rts2 packet sent");

	// flush the outlet
	Outlet::flush(timekey);
}

} // namespace meteo
