#ifndef _RETF_H
#define _RETF_H	1

#include "cpps.h"

#define MAKE_RETCP(c, v) (std::make_pair(((bool)(c)), (v)))
#define MKRCP MAKE_RETCP
#define BTS(n, v) (n) = (v)
#define NOSTS else
#define STS(n, c, v) if (c) BTS((n), (v))
#define TS NOSTS STS
#define ETS NOSTS BTS

template<typename T> using retcp_t = pair<bool, T>;

template<typename TCast, typename... T> TCast retfc(TCast, T...);

#endif
