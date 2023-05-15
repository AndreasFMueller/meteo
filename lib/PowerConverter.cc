/*
 * PowerConverter.cc -- class to convert values from one system to another
 *
 * (c) 2023 Dr. Andreas Mueller, Beratung und Entwicklung
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <PowerConverter.h>
#include <MeteoException.h>
#include <mdebug.h>

namespace meteo {

double	PowerConverter::operator()(const double value, const std::string& from) const {
	if (from == getUnit())
		return value;

	if (from != "V")
		throw MeteoException("unknown Power unit `from'", from);
	if (getUnit() != "W")
		throw MeteoException("unknown Power unit `to'", getUnit());
	return value;
}

} /* namespace meteo */
