#pragma once

#include "fwd_decl.h"

#include <cstdint>

namespace obx {
namespace {

class Canary {
	protected:
		Canary();
		~Canary();
	private:
		State::Mode oldStateMode;
};

class ActionCanary : Canary {
	public:
		ActionCanary();
		~ActionCanary();
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
