#ifndef __STATE_H__
#define __STATE_H__

#include "fwd_decl.h"

#include <cstdint>
#include <unordered_set>

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
		std::unordered_set<const Autorun*> pendingAutoruns;

		void addAutorun(const Autorun* autorun);

	friend class obx::Canary;
	friend class obx::ActionCanary;
	friend class obx::ObserverCanary;
	friend class obx::Autorun;
};

extern State state;

}
}

#endif
