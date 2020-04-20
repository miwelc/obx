#ifndef __FWD_DECL_H__
#define __FWD_DECL_H__

namespace obx {
	class Observer;
	class Autorun;
	class Reaction;
	class IObservable;
	template<class T> class Observable;
	template<class T> class Computed;

	namespace __ {
		class State;
		class Canary;
		class ObserverCanary;
		class ActionCanary;
	}
}

#endif
