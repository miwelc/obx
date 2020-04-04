#ifndef __OBSERVER_CPP__
#define __OBSERVER_CPP__

#include "../Observer.h"

#include "../Observable.hpp"
#include "../Canaries.h"

namespace obx {

Observer::Observer(const std::function<void(void)>& f) : f(f) { }
Observer::Observer(std::function<void(void)>&& f) : f(std::move(f)) { }

Observer::~Observer() { stopObserving(); }

void Observer::observe() const {
	ObserverCanary observerCanary(this);
	stopObserving();
	f();
	tainted = false;
}

void Observer::stopObserving() const {
	const auto end = observables.end();
	for(auto it = observables.begin(); it != end; it = observables.erase(it)) {
		(*it)->removeObserver(this);
	}
}


bool Observer::isTainted() const { return tainted; }
void Observer::markAsTainted() const { tainted = true; }

// Protected

void Observer::addObservable(const IObservable* observable) const {
	observables.insert(observable);
}

}

#endif
