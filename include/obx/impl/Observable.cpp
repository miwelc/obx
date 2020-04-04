#ifndef __OBSERVABLE_CPP__
#define __OBSERVABLE_CPP__

#include "../observable.hpp"

#include "../State.h"
#include "../Observer.h"

namespace obx {

bool IObservable::isBeingObserved() const {
	return !observers.empty();
}

void IObservable::markAsObserved() const {
	if(obx::state.isInObserver()) {
		const Observer* observerPtr = obx::state.currentObserver();
		addObserver(observerPtr);
		observerPtr->addObservable(this);
	}
}

void IObservable::invalidateObservers() const {
	for(auto observerPtr : observers) {
		observerPtr->markAsTainted();
	}
}

IObservable::IObservable() { }

void IObservable::addObserver(const Observer* observer) const {
	observers.insert(observer);
}

void IObservable::removeObserver(const Observer* observer) const {
	observers.erase(observer);
}

}

#endif
