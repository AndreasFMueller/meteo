/*
 * EnergyConverter.cc -- class to convert values from one system to another
 *
 * (c) 2023 Dr. Andreas Mueller, Beratung und Entwicklung
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <EnergyConverter.h>
#include <MeteoException.h>
#include <mdebug.h>

namespace meteo {

double	EnergyConverter::operator()(const double value, const std::string& from) const {
	if (from == getUnit())
		return value;

	if (from != "J")
		throw MeteoException("unknown Energy unit `from'", from);
	if (getUnit() != "J")
		throw MeteoException("unknown Energy unit `to'", getUnit());
	return value;
}

} /* namespace meteo */
