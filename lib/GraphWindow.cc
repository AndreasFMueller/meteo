/*
 * GraphWindow.cc -- implementation of graphing functions
 *
 * (c) 2003 Dr. Andreas Mueller, Beratung und Entwicklung
 *
 * $Id: GraphWindow.cc,v 1.17 2009/01/10 19:00:24 afm Exp $
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#ifdef HAVE_STRING_H
#include <string.h>
#endif /* HAVE_STRING_H */
#include <GraphWindow.h>
#include <Configuration.h>
#include <mdebug.h>
#include <gdfonts.h>
#include <Dataset.h>

namespace meteo {

// method to reduce the time to the grid. Note that the reduced time
// satisfies the following equation:
//
//      (t - ((t + offset) % interval)) + offset
//    = t+offset - (t+offset)%interval = 0 mod interval
//
// i.e. the reduced time is always such that t+offset is a multiple
// of the interval, i.e. is a timekey. Furthermore, the equation shows
// that the reduceTime operation is idempotent.
//
// as long as the offset is divisible by the interval (for intervals
// 300 and 1800 satisfied), the reduction does nothing, but for larger
// intervals, the difference is large. E.g. for Lajeado, we have
// offset = -10800, so time t=0 reduces to -10800%7200 = -3600, so
// the offset leads to an offset of one hour.
time_t	GraphWindow::reduceTime(time_t t) const {
	return t - ((t + offset) % interval);
}

// getPoint	get apoint from a graphpoint
Point	GraphWindow::getPoint(bool useleftscale, const GraphPoint& gp) const {
	return Point(getXFromTime(gp.getTime()), getY(useleftscale, gp.getValue()));
}

// getBottomPoint and getTopPoint -- get a point based on a time value
Point	GraphWindow::getBottomPoint(time_t t) const {
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "reduce: %d to %d, x = %d", t,
		reduceTime(t), getXFromTime(t));
	return Point(getXFromTime(t), window.getLow());
}
Point	GraphWindow::getTopPoint(time_t t) const {
	return Point(getXFromTime(t), window.getHigh());
}

// drawLine -- draw a line joining the data points of data in the specified
//             color. If a datapoint is missing, interrupt the graph unless
//             the connected flag is set
void	GraphWindow::drawLine(bool useleftscale, const Tdata& data,
		const Color& color, linestyle style, bool connected,
		bool antialiased) {
	GraphPoint	previous;
	bool		haveprevious = false;
	tdata_t::const_iterator	j;

	for (int i = 0; i < getWidth(); i++) {
		time_t	timeindex = getTimeFromIndex(i);
		// look for data based on timekey = time + offset
		j = data.find(timeindex + offset);
		if (j == data.end()) {
			if (!connected)
				haveprevious = false;
			mdebug(LOG_DEBUG, MDEBUG_LOG, 0,
				"no data for time %d, key %d",
				timeindex, timeindex + offset);
		} else {
			GraphPoint	current(timeindex, j->second);
			// draw a line if the previous point is set
			if (haveprevious) {
				mdebug(LOG_DEBUG, MDEBUG_LOG, 0,
					"drawing data for time %d", timeindex);
				drawLine(useleftscale, previous, current,
					color, style, antialiased && antialias);
			}
			previous = current;
			haveprevious = true;
		}
	}
}

// drawHistogram -- draw vertical lines from the data point to the bottom
//                  of the graph in the specified color (used for solar
//                  radiation and wind)
void	GraphWindow::drawHistogram(bool useleftscale, const Tdata& data,
		const Color& color) {
	tdata_t::const_iterator	i;
	for (i = data.begin(); i != data.end(); i++) {
		// since data always uses timekeys, not time, we must correct
		// for the offset where we need time
		time_t	timekey = i->first;
		parent.drawLine(getBottomPoint(timekey - offset),
			getPoint(useleftscale,
				GraphPoint(timekey - offset, i->second)),
			color, solid);
	}
}

// drawRange -- draw the range between the two data sets lower and upper in
//              the specified color (used for temperature, humidity and
//              pressure graphs)
void	GraphWindow::drawRange(bool useleftscale, const Tdata& lower,
		const Tdata& upper, const Color& color) {
	tdata_t::const_iterator	i, j;
	for (i = lower.begin(); i != lower.end(); i++) {
		time_t	timekey = i->first;
		j = upper.find(timekey);
		if (j != upper.end()) {
			mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "data for time %d",
				timekey - offset);
			drawLine(useleftscale,
				GraphPoint(i->first - offset, i->second),
				GraphPoint(j->first - offset, j->second),
				color, solid, antialias);
		}
	}
}

// drawNodata -- for each time point where we don't have any data, write
//               a vertical line over the graph with the color specified
//               (usually gray)
void	GraphWindow::drawNodata(const Tdata& data, const Color& color) {
	for (int i = 0; i <= getWidth(); i++) {
		time_t	t = getTimeFromIndex(i);
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "nodata at %d->%d", i, t);
		// look for the data based on the timekey (datasets always
		// use timekey) timekey = time + offset
		if (data.end() == data.find(t + offset)) {
			parent.drawLine(getTopPoint(t),
				getBottomPoint(t), color, solid);
		}
	}
}

// drawWindBackground -- based on a scale and for colors, draw background
//                       for the wind graph the illustrates somewhat more
//                       easily the wind direction
void	GraphWindow::drawWindBackground(bool useleftscale,
		const Color& northcolor, const Color& southcolor,
		const Color& westcolor, const Color& eastcolor) {
	// wind from south
	drawRectangle(useleftscale,
		GraphPoint(starttime, 0.), GraphPoint(endtime, 45.),
		southcolor);
	drawRectangle(useleftscale,
		GraphPoint(starttime, 315.), GraphPoint(endtime, 360.),
		southcolor);
	// wind from north
	drawRectangle(useleftscale,
		GraphPoint(starttime, 135.), GraphPoint(endtime, 225.),
		northcolor);
	// wind from east
	drawRectangle(useleftscale,
		GraphPoint(starttime, 225.), GraphPoint(endtime, 315.),
		eastcolor);
	// wind from west
	drawRectangle(useleftscale,
		GraphPoint(starttime, 45.), GraphPoint(endtime, 135.),
		westcolor);
}

void	GraphWindow::drawWindBackgroundLetters(bool useleftscale,
		const std::string& letters) {
	int	step = (3 * gdFontSmall->w * interval) / 2;
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "drawing wind background letters %s",
		letters.c_str());

	// draw letters between starttime + step and endtime - step
	for (time_t t = starttime + step; t < endtime - step; t += 2 * step) {
		// draw a letter for each angle value, draw angles 0. and 360.
		// so that half that letter gets displayed in any case
		for (int i = 0; i <= 4; i++) {
			double azi = i * 90.;
			drawLetter(useleftscale, letters.data()[i % 4],
				GraphPoint(t, azi), parent.getBackground());
		}
	}
}

static bool	startOfHalfhour(time_t t, int interval, struct tm *tmp) {
	struct tm	*lt;
	if (NULL == tmp) {
		if (interval < 3600)
			t -= t % interval;
		else
			t -= t % 3600;
		lt = localtime(&t);
	} else
		lt = tmp;
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "startOfHour(%d): tm_hour = %d, "
		"tm_min = %d, tm_sec = %d", t, lt->tm_hour, lt->tm_min,
		lt->tm_sec);
	return (((lt->tm_min == 0) || (lt->tm_min == 30)) && (lt->tm_sec == 0));
}

static bool	startOfHour(time_t t, int interval, struct tm *tmp) {
	struct tm	*lt;
	if (NULL == tmp) {
		if (interval < 3600)
			t -= t % interval;
		else
			t -= t % 3600;
		lt = localtime(&t);
	} else
		lt = tmp;
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "startOfHour(%d): tm_hour = %d, "
		"tm_min = %d, tm_sec = %d", t, lt->tm_hour, lt->tm_min,
		lt->tm_sec);
	return ((lt->tm_min == 0) && (lt->tm_sec == 0));
}

static bool	startOfEvenHour(time_t t, int interval, struct tm *tmp) {
	struct tm	*lt;
	if (NULL == tmp) {
		t -= t % interval;
		lt = localtime(&t);
	} else
		lt = tmp;
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "startOfEvenHour(%d): tm_hour = %d, "
		"tm_min = %d, tm_sec = %d", t, lt->tm_hour, lt->tm_min,
		lt->tm_sec);
	return (((lt->tm_hour % 2) == 0) && (lt->tm_min == 0) && (lt->tm_sec == 0));
}

static bool	startOfDay(time_t t, int interval, struct tm *tmp) {
	struct tm	*lt;
	if (NULL == tmp) {
		t -= t % interval;
		lt = localtime(&t);
	} else
		lt = tmp;
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "startOfDay(%d): tm_hour = %d, "
		"tm_min = %d", t, lt->tm_hour, lt->tm_min);
	return ((lt->tm_hour == 0) && (lt->tm_min == 0));
}

static bool	middleOfDay(time_t t, int interval, struct tm *tmp) {
	struct tm	*lt;
	if (NULL == tmp) {
		t -= t % interval;
		lt = localtime(&t);
	} else
		lt = tmp;
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "middleOfDay: tm_hour = %d, "
		"tm_min = %d", lt->tm_hour, lt->tm_min);
	return ((lt->tm_hour == 12) && (lt->tm_min == 0));
}

// weeks start on sunday
static bool	startOfWeek(time_t t, int interval, struct tm *tmp) {
	struct tm	*lt;
	if (NULL == tmp) {
		t -= t % interval;
		lt = localtime(&t);
	} else
		lt = tmp;
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "startOfWeek(%d): tm_wday = %d, "
		"tm_hour = %d, tm_min = %d", t, lt->tm_wday, lt->tm_hour,
		lt->tm_min);
	return (((lt->tm_hour == 0) || (lt->tm_hour == 1)) && (lt->tm_wday == 0));
}

static bool	middleOfWeek(time_t t, int interval, struct tm *tmp) {
	struct tm	*lt;
	if (NULL == tmp) {
		t -= t % interval;
		lt = localtime(&t);
	} else
		lt = tmp;
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "middleOfWeek(%d): tm_wday = %d, "
		"tm_hour = %d, tm_min = %d", t, lt->tm_wday, lt->tm_hour,
		lt->tm_min);
	return (((lt->tm_hour == 12) || (lt->tm_hour == 13)) && (lt->tm_wday == 3));
}

static bool	startOfMonth(time_t t, int interval, struct tm *tmp) {
	struct tm	*lt;
	if (NULL == tmp) {
		t -= t % interval;
		lt = localtime(&t);
	} else
		lt = tmp;
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "startOfMonth(%d): tm_wday = %d, "
		"tm_hour = %d, tm_min = %d", t, lt->tm_mday, lt->tm_hour,
		lt->tm_min);
	return (lt->tm_mday == 1);
}

static bool	middleOfMonth(time_t t, int interval, struct tm *tmp) {
	struct tm	*lt;
	if (NULL == tmp) {
		t -= t % interval;
		lt = localtime(&t);
	} else
		lt = tmp;
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "middleOfMonth(%d): tm_mday = %d, "
		"tm_hour = %d, tm_min = %d", t, lt->tm_mday, lt->tm_hour,
		lt->tm_min);
	return (lt->tm_mday == 15);
}

MapArea	GraphWindow::getArea(time_t start, time_t end) const {
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "adding area %d - %d", start, end);
	// compute dimensions for the rectangle, but convert them to the
	// usual web coordinate system for image maps, which is upside down
	// from the gd coordinate system.
	Point	ll(getXFromTime(start), parent.getHeight() - window.getLow());
	Point	ur(getXFromTime(end), parent.getHeight() - window.getHigh());
	Rectangle r(ll, ur);

	// compute mid time for this area
	Level	l(interval);
	// we compute the averages of start and end, but since the sum of the
	// two will overflow an integer, we would end up with an integer.
	// Therefore we have to devide by 2 first, and add afterwards!
	Timelabel	tl(start/2 + end/2, l.down());

	return MapArea(r, tl.getString(), start);
}

// drawTimeGrid -- draw the time grid for this resolution, note that time 
//                 is drawn in local time, while the time_t arguments are
//                 UTC, so we cannot just use draw a tick whenever the remainder
//                 of t divided by a suitable multible of the interval is 0
void	GraphWindow::drawVerticalLine(time_t t, linestyle style) {
	parent.drawLine(Point(getXFromTime(t), window.getHigh()),
		Point(getXFromTime(t), window.getLow()),
		parent.getForeground(), style);
}
void	GraphWindow::drawTimeTick(time_t t) {
	parent.drawLine(Point(getXFromTime(t), window.getLow() + 2),
		Point(getXFromTime(t), window.getLow() - 2),
		parent.getForeground(), solid);
}
void	GraphWindow::drawTimeLabel(time_t t, const std::string& label) {
	int	x;
	// find the length of the string and compute the width
	x = (int)(getXFromTime(t) - (0.5 * gdFontSmall->w * label.length()));
	Point	start(x, window.getLow() - 5);

	// draw the label
	parent.drawText(label, start, parent.getForeground(), true);
}
void	GraphWindow::drawTimeGrid(void) {
	char	label[32];
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "creating time grid");
	time_t	areastart = starttime;
	for (time_t t = starttime; t <= endtime; t += interval) {
		struct tm	*tmp = localtime(&t);
		struct tm	lt;
		memcpy(&lt, tmp, sizeof(lt));
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "drawTimeGrid time %d", t);
		switch (interval) {
		case 60:
			if (startOfHalfhour(t, interval, &lt)) {
				drawVerticalLine(t, dotted);
				drawTimeTick(t);
			}
			if (startOfHour(t, interval, &lt)) {
				strftime(label, sizeof(label), "%H", &lt);
				drawTimeLabel(t, label);
			}
			break;
		case 300:
			if (startOfHour(t, interval, &lt)) {
				drawVerticalLine(t, dotted);
				drawTimeTick(t);
			}
			if (startOfEvenHour(t, interval, &lt)) {
				strftime(label, sizeof(label), "%H", &lt);
				drawTimeLabel(t, label);
			}
			break;
		case 1800:
			if (startOfDay(t, interval, &lt)) {
				drawVerticalLine(t, dotted);
				drawTimeTick(t);
				imap.addArea(getArea(areastart, t));
				areastart = t;
			}
			if (middleOfDay(t, interval, &lt)) {
				strftime(label, sizeof(label), "%a", &lt);
				drawTimeLabel(t, label);
			}
			break;
		case 7200:
			if (startOfWeek(t, interval, &lt)) {
				drawVerticalLine(t, dotted);
				drawTimeTick(t);
				imap.addArea(getArea(areastart, t));
				areastart = t;
			}
			if (middleOfWeek(t, interval, &lt)) {
				strftime(label, sizeof(label), "week %V", &lt);
				drawTimeLabel(t, label);
			}
			break;
		case 86400:
			if (startOfMonth(t, interval, &lt)) {
				drawVerticalLine(t, dotted);
				drawTimeTick(t);
				imap.addArea(getArea(areastart, t));
				areastart = t;
			}
			if (middleOfMonth(t, interval, &lt)) {
				strftime(label, sizeof(label), "%b", &lt);
				drawTimeLabel(t, label);
			}
			break;
		}
	}
	// add image map area for last displayed data
	if (interval > 300)
		imap.addArea(getArea(areastart, endtime));
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "time grid complete");
}

// drawValueGrid -- draw the horizontal grid lines, the ticks and the
//                  value labels
void	GraphWindow::drawValueGridlines(bool useleftscale) {
	// first find out which axis object to use for the grid
	Axis	ax = (useleftscale) ? leftaxis : rightaxis;

	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "drawing value grid from %f to %f, "
		"step %f", ax.getFirst(), ax.getLast(), ax.getStep());

	// draw the dotted lines and tick marks
	for (double gridvalue = ax.getFirst(); gridvalue <= ax.getLast();
		gridvalue += ax.getStep()) {
		mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "grid value for %f",
			gridvalue);
		int	y = getY(useleftscale, gridvalue);

		// a dotted line
		parent.drawLine(Point(window.getLeft(), y),
			Point(window.getRight(), y), parent.getForeground(),
			dotted);
	}
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "done with value grid");
}

void	GraphWindow::drawValueTicks(bool useleftscale) {
	// first find out which axis object to use for the grid
	Axis	ax = (useleftscale) ? leftaxis : rightaxis;

	// draw the dotted lines and tick marks
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "drawing ticks/labels from %f to %f, "
		"step %f, label format %s", ax.getFirst(), ax.getLast(),
		ax.getStep(), ax.getFormat().c_str());
	for (double gridvalue = ax.getFirst(); gridvalue <= ax.getLast();
		gridvalue += ax.getStep()) {
		// select the right points (left/right) for ticks
		int	x, y = getY(useleftscale, gridvalue);
		if (useleftscale) {
			x = window.getLeft() - 1;
		} else {
			x = window.getRight() + 1;
		}

		// the tick mark
		parent.drawLine(Point(x - 2, y), Point(x + 2, y),
			parent.getForeground(), solid);

		// place the tick label (place it half a character height
		// below the tick line and at the width of the tick label string
		// to the left
		char	label[32];
		snprintf(label, sizeof(label), ax.getFormat().c_str(),
			gridvalue);
		y += (int)(0.5 * gdFontSmall->h);
		if (useleftscale) {
			int	w = gdFontSmall->w * strlen(label);
			parent.drawText(std::string(label),
				Point(x - w - 3, y), parent.getForeground(),
				true);
		} else {
			parent.drawText(std::string(label),
				Point(x + 3, y), parent.getForeground(),
				true);
		}
	}
	mdebug(LOG_DEBUG, MDEBUG_LOG, 0, "done with value ticks");
}

void	GraphWindow::drawValueGrid(void) {
	if (leftaxis.getGridlines())
		drawValueGridlines(true);
	if (leftaxis.getTicks())
		drawValueTicks(true);
	if (rightaxis.getGridlines())
		drawValueGridlines(false);
	if (rightaxis.getTicks())
		drawValueTicks(false);
}

// draw a frame around the graph
void	GraphWindow::drawFrame(void) {
	parent.drawLine(Point(window.getLeft() - 1, window.getLow()),
		Point(window.getRight() + 1, window.getLow()),
		parent.getForeground(), solid);
	parent.drawLine(Point(window.getLeft() - 1, window.getHigh()),
		Point(window.getRight() + 1, window.getHigh()),
		parent.getForeground(), solid);
	parent.drawLine(Point(window.getLeft() - 1, window.getLow()),
		Point(window.getLeft() - 1, window.getHigh()),
		parent.getForeground(), solid);
	parent.drawLine(Point(window.getRight() + 1, window.getLow()),
		Point(window.getRight() + 1, window.getHigh()),
		parent.getForeground(), solid);
}

// return string form of the image map
std::string	GraphWindow::mapString(const std::string& url) const {
	return imap.getStringForm(url);
}

std::string	GraphWindow::imageTagString(const std::string& filename) const {
	return imap.getImageTag(filename);
}

} /* namespace meteo */
