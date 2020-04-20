#ifndef __UTILS_CPP__
#define __UTILS_CPP__

#include "../utils.h"

#include "../Canaries.h"

namespace obx {

OBX_INLINE void runInAction(const std::function<void(void)>& f) {
	Action _;
	f();
}

}

#endif
