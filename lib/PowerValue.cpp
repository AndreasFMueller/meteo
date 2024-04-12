/*
 * PowerValue.cc -- implement power
 *
 * (c) 2023 Dr. Andreas Mueller, Beratung und Entwicklung
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <PowerValue.h>
#include <MeteoException.h>
#include <PowerConverter.h>
#include <mdebug.h>

namespace meteo {

// PowerValue methods
PowerValue::PowerValue(void) : BasicValue("W") {
}
PowerValue::PowerValue(double v) : BasicValue("W") {
	setValue(v);
}

PowerValue::PowerValue(double v, const std::string& u)
	: BasicValue(u) {
	setValue(v);
}

PowerValue::PowerValue(const std::string& u) : BasicValue(u) {
}

void	PowerValue::setUnit(const std::string& targetunit) {
	if (hasValue()) {
		PowerConverter(targetunit).convert(this);
	}
}

} /* namespace meteo */
