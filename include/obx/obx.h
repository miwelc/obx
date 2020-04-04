#pragma once

#ifdef OBX_AS_LIB
	#define OBX_INLINE
#else
	#define OBX_INLINE inline
#endif


#include "State.h"
#include "Observable.hpp"
#include "Observer.h"
#include "Computed.hpp"
#include "Canaries.h"
#include "utils.h"

#ifndef OBX_AS_LIB

#include "impl/State.cpp"
#include "impl/Observable.cpp"
#include "impl/Observer.cpp"
#include "impl/Canaries.cpp"
#include "impl/utils.cpp"

#endif
