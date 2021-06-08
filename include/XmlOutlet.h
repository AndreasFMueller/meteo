/*
 * XmlOutlet.h -- implement sending stuff to an XML file
 *
 * (c) 2008 Prof Dr Andreas Mueller
 */
#include <Outlet.h>
#include <XmlOutletDelegate.h>
#include <string>

namespace meteo {

class XmlOutlet : public Outlet {
	std::string	stationname;
	XmlOutletDelegate	*delegate;
public:
	XmlOutlet(const std::string& stationname);
	XmlOutlet(const std::string& stationname, XmlOutletDelegate *_delegate);
	virtual	~XmlOutlet();
	void	flush(const time_t timekey);
};

} // namespace meteo
