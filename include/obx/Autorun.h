#ifndef __AUTORUN_H__
#define __AUTORUN_H__

#include "Observer.h"

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

	private:
		void run() const;
		friend class obx::ActionCanary;
		friend class obx::State;
};

}

#endif
