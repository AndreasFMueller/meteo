//
// Mapfile.cc -- class used to send current station data to a map file
//
// (c) 2004 Dr. Andreas Mueller, Beratung und Entwicklung
// $Id: Mapfile.cc,v 1.5 2009/01/10 19:00:24 afm Exp $
//
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <Mapfile.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#ifdef HAVE_STRING_H
#include <string.h>
#endif /* HAVE_STRING_H */
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif /* HAVE_FCNTL_H */
#include <mdebug.h>
#include <MeteoException.h>
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif /* HAVE_ERRNO_H */
#include <Format.h>
#include <Timestamp.h>

namespace meteo {

// this should be large enough for all currently known davis stations
int	Mapfile::mapsize = 2048;

Mapfile::Mapfile(const std::string& filename, bool writable) {
	int	flags = O_RDONLY;
	if (writable) {
		flags = O_CREAT | O_TRUNC | O_RDWR;
	}
	// open and initialize the map file 
	mapfile = open(filename.c_str(), flags, 0666);
	if (mapfile < 0) {
		mdebug(LOG_ERR, MDEBUG_LOG, 0, "cannot open map file %s: %s",
			filename.c_str(), strerror(errno));
		throw MeteoException("cannot open map file", strerror(errno));
	}
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "map file %s created",
		filename.c_str());
}

// publish the data in the scratch area
void	Mapfile::publish(void) {
	Timestamp	t;
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "publishing meteo data");
	std::string	result("<?xml version=\"1.0\"?>\n");
	stringprintf(result, "<meteomap station=\"%s\" timestamp=\"%s\"",
		stationname.c_str(), t.ctime().c_str());
	result.append(t.strftime(" day=\"%d\" month=\"%m\" year=\"%Y\" hour=\"%H\" minute=\"%M\" second=\"%S\""));
	result.append(">\n");

	// write the data in the data to the scratch area
	mapdata_t::iterator	i;
	for (i = data.begin(); i != data.end(); i++) {
		char	b[1024];
		snprintf(b, sizeof(b),
			"<data name=\"%s\" value=\"%.3f\" unit=\"%s\"/>\n",
			i->first.c_str(), i->second.first,
			i->second.second.c_str());
		result.append(b);
	}
	result.append("</meteomap>\n");

	// rewind to the beginning of the map
	if (lseek(mapfile, 0, SEEK_SET) < 0) {
		mdebug(LOG_ERR, MDEBUG_LOG, 0, "cannot rewind map: %s",
			strerror(errno));
		throw MeteoException("cannot rewind the map file",
			strerror(errno));
	}

	// fill a buffer with the data from the string plus spaces
	char	buffer[mapsize];
	memset(buffer, ' ', mapsize);
	buffer[mapsize - 1] = '\n';
	memcpy(buffer, result.data(), result.length());

	// perform an atomic write to the mapfile
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "writing %d bytes to mapfile", mapsize);
	if (mapsize != write(mapfile, buffer, mapsize)) {
		mdebug(LOG_ERR, MDEBUG_LOG, 0, "cannot write map: %s",
			strerror(errno));
		throw MeteoException("cannot write map", strerror(errno));
	}
}

// read the data from the map file
std::string	Mapfile::read(void) {
	char	buffer[mapsize];
	int	bytes = ::read(mapfile, buffer, mapsize);
	if (mapsize != bytes) {
		// this really shouldn't happen
		mdebug(LOG_WARNING, MDEBUG_LOG, 0,
			"mapfile incomlete only %d of %d bytes",
			bytes, mapsize);
	}
	return std::string(buffer, mapsize);
}

// add a named value to the mapfile
void	Mapfile::add(const std::string& name, double value,
	const std::string& unit) {
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "Mapfile::add(%s -> %.f%s)",
		name.c_str(), value, unit.c_str());
	data.insert(mapdata_t::value_type(name, mapvalue_t(value, unit)));
}

// clear the data map
void	Mapfile::clear(void) {
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "clearing map file");
	data.clear();	// shortcut for data.erase(data.begin(), data.end());
}

} /* namespace meteo */
