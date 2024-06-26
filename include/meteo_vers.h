/*
**  meteo_vers.h -- Version Information for Meteo (syntax: C/C++)
**  [automatically generated and maintained by GNU shtool]
*/

#ifdef _METEO_VERS_H_AS_HEADER_

#ifndef _METEO_VERS_H_
#define _METEO_VERS_H_

#define METEO_VERSION 0x009218

typedef struct {
    const int   v_hex;
    const char *v_short;
    const char *v_long;
    const char *v_tex;
    const char *v_gnu;
    const char *v_web;
    const char *v_sccs;
    const char *v_rcs;
} meteo_version_t;

extern meteo_version_t meteo_version;

#endif /* _METEO_VERS_H_ */

#else /* _METEO_VERS_H_AS_HEADER_ */

#define _METEO_VERS_H_AS_HEADER_
#include "meteo_vers.h"
#undef  _METEO_VERS_H_AS_HEADER_

meteo_version_t meteo_version = {
    0x009218,
    "0.9.24",
    "0.9.24 (12-Apr-2024)",
    "This is Meteo, Version 0.9.24 (12-Apr-2024)",
    "Meteo 0.9.24 (12-Apr-2024)",
    "Meteo/0.9.24",
    "@(#)Meteo 0.9.24 (12-Apr-2024)",
    "$Id: shtool,v 1.1 2001/03/18 15:07:34 afm Exp Meteo 0.9.24 (12-Apr-2024) $"
};

#endif /* _METEO_VERS_H_AS_HEADER_ */

