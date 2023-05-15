/*
 * PowerRecorder.h -- encapsulate voltage
 *
 * (c) 2023 Dr. Andreas Mueller, Beratung und Entwicklung
 */
#ifndef _PowerRecorder_h
#define _PowerRecorder_h

#include <MinmaxRecorder.h>
#include <string>

namespace meteo {

class	PowerRecorder : public BasicRecorder {
public:
	PowerRecorder(void);
	PowerRecorder(double);
	PowerRecorder(double, const std::string&);
	PowerRecorder(const std::string&);
	virtual ~PowerRecorder(void) { }
	virtual std::string	getValueClass(void) const {
		return "PowerValue";
	}
};

} // namespace meteo

#endif /* _PowerRecorder_h */
