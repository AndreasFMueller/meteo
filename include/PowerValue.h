/*
 * PowerValue.h -- encapsulate power
 *
 * (c) 2023 Dr. Andreas Mueller, Beratung und Entwicklung
 */
#ifndef _PowerValue_h
#define _PowerValue_h

#include <BasicValue.h>
#include <Timeval.h>
#include <string>

namespace meteo {

class	PowerValue : public BasicValue {
public:
	PowerValue(void);
	PowerValue(double T);
	PowerValue(double T, const std::string& u);
	PowerValue(const std::string& u);
	virtual ~PowerValue(void) { }
	virtual void	setUnit(const std::string& u);
	virtual std::string	getClass(void) const {
		return "PowerValue";
	}
};

} // namespace meteo

#endif /* _PowerValue_h */
