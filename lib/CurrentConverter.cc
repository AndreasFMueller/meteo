/*
 * CurrentConverter.cc -- class to convert values from one system to another
 *
 * (c) 2003 Dr. Andreas Mueller, Beratung und Entwicklung
 *
 * $Id: CurrentConverter.cc,v 1.3 2009/01/10 19:00:25 afm Exp $
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <CurrentConverter.h>
#include <MeteoException.h>
#include <mdebug.h>

namespace meteo {

double	CurrentConverter::operator()(const double value, const std::string& from) const {
	if (from == getUnit())
		return value;

	if (from != "V")
		throw MeteoException("unknown Current unit `from'", from);
	if (getUnit() != "V")
		throw MeteoException("unknown Current unit `to'", getUnit());
	return value;
}

} /* namespace meteo */
