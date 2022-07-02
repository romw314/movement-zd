#include "retf.h"

template<typename TCast, typename... T> TCast retfc(TCast def, T... pairs) {
	return (([](retcp_t<TCast> x){
		if (x.first)
			return (TCast)x.second;
	}((retcp_t<TCast>)pairs)), ...);
	return def;
}
