/*
 * EnergyConverter.h -- convert energy values
 *
 * (c) 2023 Dr. Andreas Mueller, Beratung und Entwicklung
 */
#ifndef _EnergyConverter_h
#define _EnergyConverter_h

#include <string>
#include <BasicConverter.h>
#include <EnergyValue.h>

namespace meteo {

class	EnergyConverter : public BasicConverter {
public:
	EnergyConverter(void) : BasicConverter("J") { }
	EnergyConverter(const std::string& unit) : BasicConverter(unit) { }
	virtual ~EnergyConverter(void) { }
	virtual double	operator()(const double value, const std::string& from) const;
};

} /* namespace meteo */

#endif /* _EnergyConverter_h */
