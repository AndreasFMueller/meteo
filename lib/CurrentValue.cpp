/*
 * CurrentValue.cc -- implement voltage (for VantagePro console battery)
 *
 * (c) 2003 Dr. Andreas Mueller, Beratung und Entwicklung
 *
 * $Id: CurrentValue.cc,v 1.3 2009/01/10 19:00:25 afm Exp $
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <CurrentValue.h>
#include <MeteoException.h>
#include <CurrentConverter.h>
#include <mdebug.h>

namespace meteo {

// CurrentValue methods
CurrentValue::CurrentValue(void) : BasicValue("A") {
}
CurrentValue::CurrentValue(double v) : BasicValue("A") {
	setValue(v);
}

CurrentValue::CurrentValue(double v, const std::string& u)
	: BasicValue(u) {
	setValue(v);
}

CurrentValue::CurrentValue(const std::string& u) : BasicValue(u) {
}

void	CurrentValue::setUnit(const std::string& targetunit) {
	if (hasValue()) {
		CurrentConverter(targetunit).convert(this);
	}
}

} /* namespace meteo */
