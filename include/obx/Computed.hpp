#ifndef __COMPUTED_H__
#define __COMPUTED_H__

#include "fwd_decl.h"

#include "log.h"

namespace obx {

template<class T>
class Computed : protected Observer, protected IObservable {
	public:
		Computed(const std::function<T(void)>& f)
			: Observer([this, f]() { val = f(); }) { }
		Computed(std::function<T(void)>&& f)
			: Observer([this, f = std::move(f)]() { val = f(); }) { }
		Computed(Computed&&) = delete;
		Computed(const Computed&) = delete;

		const T& operator()() {
			log<LogLevel::DEBUG>("\t\tRead Computed\n");
			if(Observer::tainted) {
				log<LogLevel::DEBUG>("\t\tComputing\n");
				Observer::observe();
			}
			IObservable::markAsObserved();
			return val;
		}
		operator const T&() { return this->operator()(); }

	protected:
		virtual void markAsTainted() const override {
			Observer::markAsTainted();
			IObservable::invalidateObservers();
		}

	private:
		T val;
};

}

#endif
