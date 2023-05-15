/*
 * PowerRecoder.cc -- encapsulate current measurements
 *
 * (c) 2023 Dr. Andreas Mueller, Beratung und Entwicklung
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <PowerRecorder.h>
#include <MeteoException.h>
#include <PowerConverter.h>
#include <mdebug.h>

namespace meteo {

PowerRecorder::PowerRecorder(void) : BasicRecorder("W") { }

PowerRecorder::PowerRecorder(double v) : BasicRecorder("W") {
	setValue(v);
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "initialized current from %.2fV", v);
}

PowerRecorder::PowerRecorder(double v, const std::string& u) : BasicRecorder(u) {
	setValue(v);
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "initialized current from %.2f%s",
		v, u.c_str());
}

PowerRecorder::PowerRecorder(const std::string& u) : BasicRecorder(u) {
}

} /* namespace meteo */
