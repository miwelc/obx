#ifndef __STATE_CPP__
#define __STATE_CPP__

#include "../State.h"

namespace obx {
namespace {

const Observer* State::currentObserver() const {
	return observerPtr;
}

inline State state{};

}
}

#endif
