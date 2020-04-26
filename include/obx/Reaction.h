#ifndef __REACTION_H__
#define __REACTION_H__

#include "Autorun.h"

#include <functional>

namespace obx {

class Reaction : public Autorun {
	public:
		using CB = std::function<void(void)>;

		Reaction(const CB& autorunCB, const CB& then, bool enabled = true);
		Reaction(CB&& autorunCB, CB&& then, bool enabled = true);
		Reaction(CB&& autorunCB, const CB& then, bool enabled = true);
		Reaction(const CB& autorunCB, CB&& then, bool enabled = true);
		Reaction(Reaction&&) = default;
		Reaction(const Reaction&) = delete;

		virtual ~Reaction() = default;

	private:
		CB then;

		virtual void run() const override;
};

}

#endif
