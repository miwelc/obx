#ifndef __UTILS_CPP__
#define __UTILS_CPP__

#include "../utils.h"

#include "../Canaries.h"

#include <functional>

namespace obx {

OBX_INLINE void runInAction(const std::function<void(void)>& f) {
	ActionCanary actionCanary;
	f();
}

}

#endif
