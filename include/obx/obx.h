#ifndef __OBX_H__
#define __OBX_H__

#if defined(OBX_AS_LIB) || defined(BUILD_OBX_LIB)
	#define OBX_INLINE
#else
	#define OBX_INLINE inline
#endif


#include "State.h"
#include "Observable.hpp"
#include "Observer.h"
#include "Computed.hpp"
#include "Autorun.h"
#include "Reaction.h"
#include "Canaries.h"
#include "utils.h"

#if !defined(OBX_AS_LIB) && !defined(BUILD_OBX_LIB)

#include "impl/State.cpp"
#include "impl/Observable.cpp"
#include "impl/Observer.cpp"
#include "impl/Autorun.cpp"
#include "impl/Reaction.cpp"
#include "impl/Canaries.cpp"
#include "impl/utils.cpp"

#endif

#endif
