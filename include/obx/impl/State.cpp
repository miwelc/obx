#ifndef __STATE_CPP__
#define __STATE_CPP__

#include "../State.h"

namespace obx {
namespace __ {

bool State::isInObserver() const {
	return mode == Mode::OBSERVER;
}

bool State::isInAction() const {
	return mode == Mode::ACTION;
}

const Observer* State::currentObserver() const {
	return observerPtr;
}

void State::addAutorun(const Autorun* autorun) {
	pendingAutoruns.insert(autorun);
}

inline State state{};

}
}

#endif
