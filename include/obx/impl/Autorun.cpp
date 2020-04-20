#ifndef __AUTORUN_CPP__
#define __AUTORUN_CPP__

#include "../Autorun.h"
#include "../State.h"

namespace obx {

Autorun::Autorun(const std::function<void(void)>& f, bool enabled) : Observer(f) {
	if(enabled) {
		enable();
	}
}

Autorun::Autorun(std::function<void(void)>&& f, bool enabled) : Observer(f) {
	if(enabled) {
		enable();
	}
}

void Autorun::enable() {
	Observer::observe();
}

void Autorun::disable() {
	Observer::stopObserving();
}

void Autorun::markAsTainted() const {
	Observer::markAsTainted();
	obx::__::state.addAutorun(this);
}

void Autorun::run() const {
	Observer::observe();
}

}

#endif
