/*
 * PowerConverter.h -- convert power values
 *
 * (c) 2023 Dr. Andreas Mueller, Beratung und Entwicklung
 */
#ifndef _PowerConverter_h
#define _PowerConverter_h

#include <string>
#include <BasicConverter.h>
#include <PowerValue.h>

namespace meteo {

class	PowerConverter : public BasicConverter {
public:
	PowerConverter(void) : BasicConverter("W") { }
	PowerConverter(const std::string& unit) : BasicConverter(unit) { }
	virtual ~PowerConverter(void) { }
	virtual double	operator()(const double value, const std::string& from) const;
};

} /* namespace meteo */

#endif /* _PowerConverter_h */
