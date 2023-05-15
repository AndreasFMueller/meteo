/*
 * CurrentConverter.h -- convert current values
 *
 * (c) 2023 Dr. Andreas Mueller, Beratung und Entwicklung
 */
#ifndef _CurrentConverter_h
#define _CurrentConverter_h

#include <string>
#include <BasicConverter.h>
#include <CurrentValue.h>

namespace meteo {

class	CurrentConverter : public BasicConverter {
public:
	CurrentConverter(void) : BasicConverter("A") { }
	CurrentConverter(const std::string& unit) : BasicConverter(unit) { }
	virtual ~CurrentConverter(void) { }
	virtual double	operator()(const double value, const std::string& from) const;
};

} /* namespace meteo */

#endif /* _CurrentConverter_h */
