#ifndef __CANARIES_CPP__
#define __CANARIES_CPP__

#include "../Canaries.h"
#include "../State.h"

#include "../Autorun.h"

#include "../log.h"

namespace obx {
namespace __ {

// Canary
Canary::Canary() : oldStateMode(obx::__::state.mode) { }
Canary::~Canary() { obx::__::state.mode = oldStateMode; }

// ActionCanary

namespace {
	inline uint32_t ActionCannaryNestingLevels = 0;
}

ActionCanary::ActionCanary() {
	if(obx::__::state.isInObserver()) {
		log<LogLevel::EXCEPTION>("Cannot run action inside an observer");
	}
	obx::__::state.mode = State::Mode::ACTION;
	ActionCannaryNestingLevels++;
}

ActionCanary::~ActionCanary() noexcept(false) {
	if(--ActionCannaryNestingLevels == 0) {
		obx::__::state.mode = Canary::oldStateMode;

		for(const auto autorun : obx::__::state.pendingAutoruns) {
			autorun->run();
		}
		obx::__::state.pendingAutoruns.clear();
	}
}

// ObserverCanary
ObserverCanary::ObserverCanary(const Observer* observer) : oldObserverPtr(obx::__::state.observerPtr) {
	obx::__::state.mode = State::Mode::OBSERVER;
	obx::__::state.observerPtr = observer;
}

ObserverCanary::~ObserverCanary() {
	obx::__::state.observerPtr = oldObserverPtr;
}

}
}

#endif
