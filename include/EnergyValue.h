/*
 * EnergyValue.h -- encapsulate energy
 *
 * (c) 2023 Dr. Andreas Mueller, Beratung und Entwicklung
 */
#ifndef _EnergyValue_h
#define _EnergyValue_h

#include <BasicValue.h>
#include <Timeval.h>
#include <string>

namespace meteo {

class	EnergyValue : public BasicValue {
public:
	EnergyValue(void);
	EnergyValue(double T);
	EnergyValue(double T, const std::string& u);
	EnergyValue(const std::string& u);
	virtual ~EnergyValue(void) { }
	virtual void	setUnit(const std::string& u);
	virtual std::string	getClass(void) const {
		return "EnergyValue";
	}
};

} // namespace meteo

#endif /* _EnergyValue_h */
