//
// Format.cc -- generic formatting routines for longs and doubles
//
// (c) 2005 Dr. Andreas Mueller, Beratung und Entwicklung
// $Id: Format.cc,v 1.4 2009/01/10 19:33:18 afm Exp $
//
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <Format.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <mdebug.h>

namespace meteo {

//
// the stringprintf function creates a std::string from a format str
// and some variable arguments
//
#define	BUFFERSIZE	1024
std::string stringvprintf(const char *format, va_list ap) {
	//char *buffer;		// should be sufficient in this application
	int bytes;
	char	buffer[BUFFERSIZE];
	bytes = vsnprintf(buffer, BUFFERSIZE, format, ap);
	if (bytes >= BUFFERSIZE) {
		// truncated, try again with the recommended buffer size
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "too small, bytes needed: %d",
			bytes);
		char	buffer2[bytes + 1];
		vsnprintf(buffer2, bytes + 1, format, ap);
		return std::string(buffer2);
	}
	return std::string(buffer);
}

std::string stringprintf(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	std::string	result = stringvprintf(format, ap);
	va_end(ap);
	return result;
}

std::string&	stringprintf(std::string& s, const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	s.append(stringvprintf(format, ap));
	va_end(ap);
	return s;
}

} // namespace meteo
