/*
 * CurrentRecoder.cc -- encapsulate current measurements
 *
 * (c) 2023 Dr. Andreas Mueller, Beratung und Entwicklung
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <CurrentRecorder.h>
#include <MeteoException.h>
#include <CurrentConverter.h>
#include <mdebug.h>

namespace meteo {

CurrentRecorder::CurrentRecorder(void) : BasicRecorder("A") { }

CurrentRecorder::CurrentRecorder(double v) : BasicRecorder("A") {
	setValue(v);
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "initialized current from %.2fV", v);
}

CurrentRecorder::CurrentRecorder(double v, const std::string& u) : BasicRecorder(u) {
	setValue(v);
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "initialized current from %.2f%s",
		v, u.c_str());
}

CurrentRecorder::CurrentRecorder(const std::string& u) : BasicRecorder(u) {
}

} /* namespace meteo */
