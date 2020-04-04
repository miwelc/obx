#ifndef __REACTION_CPP__
#define __REACTION_CPP__

#include "../Reaction.h"

namespace obx {

Reaction::Reaction(const CB& autorunCB, const CB& then, bool enabled)
	: Autorun(autorunCB, enabled), then(then) { }

Reaction::Reaction(CB&& autorunCB, CB&& then, bool enabled)
	: Autorun(std::move(autorunCB), enabled), then(std::move(then)) { }

Reaction::Reaction(CB&& autorunCB, const CB& then, bool enabled)
	: Autorun(std::move(autorunCB), enabled), then(then) { }

Reaction::Reaction(const CB& autorunCB, CB&& then, bool enabled)
	: Autorun(autorunCB, enabled), then(std::move(then)) { }


void Reaction::run() const {
	Autorun::run();
	then();
}

}

#endif
