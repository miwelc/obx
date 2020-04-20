#ifndef __AUTORUN_H__
#define __AUTORUN_H__

#include "fwd_decl.h"

#include "Observer.h"

#include <functional>

namespace obx {

class Autorun : protected Observer {
	public:
		Autorun(const std::function<void(void)>& f, bool enabled = true);
		Autorun(std::function<void(void)>&& f, bool enabled = true);
		Autorun(Autorun&&) = delete;
		Autorun(const Autorun&) = delete;

		void enable();
		void disable();

	protected:
		virtual void markAsTainted() const override;
		virtual void run() const;

	friend class obx::__::ActionCanary;
	friend class obx::__::State;
};

}

#endif
