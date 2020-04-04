#pragma once

namespace obx {
	class Observer;
	class IObservable;
	template<class T> class Observable;
	template<class T> class Computed;

	namespace {
		class State;
		class Canary;
		class ObserverCanary;
		class ActionCanary;
	}
}
