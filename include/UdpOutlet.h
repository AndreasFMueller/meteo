/*
 * UdpOutlet.h -- an outlet to send meteo data in a UDP packet
 *
 * (c) 2021 Prof Dr Andreas Müller
 */
#include <Outlet.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>

namespace meteo {

/**
 * \brief A outlet class to send meteo data to RTS2
 */
class UdpOutlet : public Outlet {
	std::string	_stationname;
	std::string	_hostname;
	unsigned short	_port;
	std::map<std::string,std::string>	_names;
public:
	const std::string&	stationname() const { return _stationname; }
	const std::string&	hostname() const { return _hostname; }
	unsigned short	port() const { return _port; }
private:
	int	_socket;
	struct sockaddr_in	_destination;
	void	setup();
	UdpOutlet(const UdpOutlet& other);
	UdpOutlet&	operator=(const UdpOutlet& other);
public:
	UdpOutlet(const std::string& stationname, const std::string& hostname,
		unsigned short port);
	virtual ~UdpOutlet();
	void	flush(const time_t timekey);
};

} // namespace meteo
