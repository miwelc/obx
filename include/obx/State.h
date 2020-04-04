#pragma once

#include "fwd_decl.h"

#include <cstdint>

namespace obx {
namespace {

class State {
	public:
		State() { }
		bool isInObserver() const { return mode == Mode::OBSERVER; }
		bool isInAction() const { return mode == Mode::ACTION; }
		const Observer* currentObserver() const;

	private:
		enum class Mode : uint32_t {
			OUTSIDE = 0,
			OBSERVER,
			ACTION
		};

		Mode mode = Mode::OUTSIDE;
		const Observer* observerPtr = nullptr;

		friend class obx::Canary;
		friend class obx::ActionCanary;
		friend class obx::ObserverCanary;
};

extern State state;

}
}
