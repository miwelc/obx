#ifndef __CANARIES_H__
#define __CANARIES_H__

#include "fwd_decl.h"

#include <cstdint>

namespace obx {

using Action = ActionCanary;

namespace {

class Canary {
	protected:
		Canary();
		~Canary();
		State::Mode oldStateMode;
};

class ActionCanary : Canary {
	public:
		ActionCanary();
		~ActionCanary() noexcept(false);
};

class ObserverCanary : Canary {
	public:
		ObserverCanary(const Observer* observer);
		~ObserverCanary();

	private:
		const Observer* oldObserverPtr;
};

}
}

#endif
