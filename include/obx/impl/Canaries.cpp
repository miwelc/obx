#ifndef __CANARIES_CPP__
#define __CANARIES_CPP__

#include "../Canaries.h"
#include "../State.h"

#include "../log.h"

namespace obx {
namespace {

// Canary
Canary::Canary() : oldStateMode(obx::state.mode) { }
Canary::~Canary() { obx::state.mode = oldStateMode; }

// ActionCanary

namespace {
	inline uint32_t ActionCannaryestingLevels = 0;
}

ActionCanary::ActionCanary() {
	if(obx::state.isInObserver()) {
		log<LogLevel::EXCEPTION>("Cannot run action inside an observer");
	}
	obx::state.mode = State::Mode::ACTION;
	ActionCannaryestingLevels++;
}

ActionCanary::~ActionCanary() {
	if(--ActionCannaryestingLevels == 0) {
		// RUN REACTIONS
	}
}

// ObserverCanary
ObserverCanary::ObserverCanary(const Observer* observer) : oldObserverPtr(obx::state.observerPtr) {
	obx::state.mode = State::Mode::OBSERVER;
	obx::state.observerPtr = observer;
}

ObserverCanary::~ObserverCanary() {
	obx::state.observerPtr = oldObserverPtr;
}

}
}

#endif
