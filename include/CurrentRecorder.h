/*
 * CurrentRecorder.h -- encapsulate voltage
 *
 * (c) 2023 Dr. Andreas Mueller, Beratung und Entwicklung
 */
#ifndef _CurrentRecorder_h
#define _CurrentRecorder_h

#include <MinmaxRecorder.h>
#include <string>

namespace meteo {

class	CurrentRecorder : public BasicRecorder {
public:
	CurrentRecorder(void);
	CurrentRecorder(double);
	CurrentRecorder(double, const std::string&);
	CurrentRecorder(const std::string&);
	virtual ~CurrentRecorder(void) { }
	virtual std::string	getValueClass(void) const {
		return "CurrentValue";
	}
};

} // namespace meteo

#endif /* _CurrentRecorder_h */
