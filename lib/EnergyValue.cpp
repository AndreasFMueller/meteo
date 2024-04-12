/*
 * EnergyValue.cc -- implement energy
 *
 * (c) 2023 Dr. Andreas Mueller, Beratung und Entwicklung
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <EnergyValue.h>
#include <MeteoException.h>
#include <EnergyConverter.h>
#include <mdebug.h>

namespace meteo {

// EnergyValue methods
EnergyValue::EnergyValue(void) : BasicValue("J") {
}
EnergyValue::EnergyValue(double v) : BasicValue("J") {
	setValue(v);
}

EnergyValue::EnergyValue(double v, const std::string& u)
	: BasicValue(u) {
	setValue(v);
}

EnergyValue::EnergyValue(const std::string& u) : BasicValue(u) {
}

void	EnergyValue::setUnit(const std::string& targetunit) {
	if (hasValue()) {
		EnergyConverter(targetunit).convert(this);
	}
}

} /* namespace meteo */
