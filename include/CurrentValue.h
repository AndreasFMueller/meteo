/*
 * CurrentValue.h -- encapsulate voltage
 *
 * (c) 2003 Dr. Andreas Mueller, Beratung und Entwicklung
 *
 * $Id: CurrentValue.h,v 1.2 2004/02/25 23:52:35 afm Exp $
 */
#ifndef _CurrentValue_h
#define _CurrentValue_h

#include <BasicValue.h>
#include <Timeval.h>
#include <string>

namespace meteo {

class	CurrentValue : public BasicValue {
public:
	CurrentValue(void);
	CurrentValue(double T);
	CurrentValue(double T, const std::string& u);
	CurrentValue(const std::string& u);
	virtual ~CurrentValue(void) { }
	virtual void	setUnit(const std::string& u);
	virtual std::string	getClass(void) const {
		return "CurrentValue";
	}
};

} // namespace meteo

#endif /* _CurrentValue_h */
